#include "../../../chickenFlap/dart/highlevel/DataStream.h"

#ifndef DEVICE_HAS_FAKE_INT8
DataStream::DataStream() {
    this->bufferSize = 0;
    this->tagsIndex = 0;
    this->dataBuffer = NULL;
    this->dataBufferIndex = 0;
    this->enabled = false;

    memset(tags, 0, sizeof(tags));
    memset(tagTypes, 0, sizeof(tagTypes));
}

DataStream::DataStream(size_t bufferSize) {
    this->bufferSize = bufferSize;
    this->tagsIndex = 0;
    this->dataBuffer = new uint8_t[bufferSize];
    this->dataBufferIndex = 0;
    this->enabled = false;

    memset(tags, 0, sizeof(tags));
    memset(tagTypes, 0, sizeof(tagTypes));

    DART_NOT_NULL(dataBuffer, DART_ERROR_INTERNAL);
}

uint8_t* DataStream::getData() {
    return dataBuffer;
}

size_t DataStream::getDataPendingSize() {
    return dataBufferIndex;
}

bool DataStream::isEnabled() {
    return enabled;
}

void DataStream::setEnabled(bool enabled) {
    this->enabled = enabled;
}

void DataStream::initTagGroup(TagGroup* group, util_time_t interval) {
    DART_NOT_NULL(group, DART_ERROR_INVALID_VALUE);

    // Create timer and copy data over
    util_timer_t timer = createTimer(interval, rand() % 1000);
    memcpy(&group->timer, &timer, sizeof(group->timer));

    // Set tagStart to no known start
    group->tagStart = 0;
}

bool DataStream::shouldLog(TagGroup* group) {
    return group == NULL || hasTimerElapsed(&group->timer);
}

void DataStream::flushData(size_t bytesWritten) {
    DART_ASSERT(bytesWritten > 0, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(bytesWritten <= dataBufferIndex, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(dataBuffer, DART_ERROR_INTERNAL);

    // Move buffer
    dataBufferIndex -= bytesWritten;
    memmove(dataBuffer, dataBuffer + bytesWritten, dataBufferIndex);
}

void DataStream::reset() {
    dataBufferIndex = 0;
}

uint16_t DataStream::findTag(TagGroup* group, const char* tag) {
    DART_NOT_NULL_RETURN(tag, DART_ERROR_INVALID_VALUE, 0);

    // Check if already the tag start is saved in the TagGroup, so
    // we don't need to search trough the entire list
    if (group != NULL && group->tagStart > 0) {
        for (int i = group->tagStart - 1; i < tagsIndex; i++)
            if (tag == tags[i])
                return (uint16_t)(i + 1);
    }

    // Find tag in buffer
    for (int i = 0; i < tagsIndex; i++)
        if (tag == tags[i])
            return (uint16_t)(i + 1);
    return 0;
}

bool DataStream::bufferNewTag(uint16_t tagID, TagType type, const char* tagName) {
    DART_ASSERT_RETURN(tagID != 0, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(tagName, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(strlen(tagName) < 64, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(dataBuffer, DART_ERROR_INTERNAL, false);

    // Ensure we have enough space
    size_t tagNameLength = strlen(tagName) + 1;
    size_t spaceNeeded = 5 + tagNameLength;
    DART_ASSERT_RETURN(dataBufferIndex + spaceNeeded < bufferSize, DART_ERROR_BUFFER_OVERFLOW, false);

    // Save tag header
    dataBuffer[dataBufferIndex] = 0;
    dataBuffer[dataBufferIndex + 1] = 0;
    dataBuffer[dataBufferIndex + 2] = (uint8_t)(tagID >> 8);
    dataBuffer[dataBufferIndex + 3] = (uint8_t)(tagID & 0xFF);
    dataBuffer[dataBufferIndex + 4] = (uint8_t)type;

    // Save tag string
    memcpy(dataBuffer + dataBufferIndex + 5, tagName, tagNameLength);
    dataBufferIndex += spaceNeeded;
    return true;
}

void DataStream::bufferTag(uint16_t tagID, uint8_t* data, size_t size) {
    DART_ASSERT(tagID > 0, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(size > 0, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(dataBuffer, DART_ERROR_INTERNAL);

    // Ensure we have enough space
    size_t spaceNeeded = 2 + size;
    DART_ASSERT(dataBufferIndex + spaceNeeded < bufferSize, DART_ERROR_BUFFER_OVERFLOW);

    // Save tag header
    dataBuffer[dataBufferIndex] = (uint8_t)(tagID >> 8);
    dataBuffer[dataBufferIndex + 1] = (uint8_t)(tagID & 0xFF);

    // Save tag data
    memcpy(dataBuffer + dataBufferIndex + 2, data, size);
    dataBufferIndex += spaceNeeded;
}


void DataStream::log(TagGroup* group, const char* tag, uint8_t* data, size_t size, TagType type) {
    DART_NOT_NULL(tag, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(size > 0, DART_ERROR_INVALID_VALUE);

    // Check if we are actually running
    if (!enabled || dataBuffer == NULL)
        return;

    Profiler::begin("DataStream::log()");

    uint16_t tagID = findTag(group, tag);
    // Write new tag packet
    if (tagID == 0) {
        // Check if we enough space in the buffer
        DART_ASSERT(tagsIndex < DATA_STREAM_MAX_TAG_COUNT, DART_ERROR_BUFFER_OVERFLOW);

        // Try to write the creation of the tag to the log
        if (!bufferNewTag(tagsIndex + 1, type, tag)) {
            Profiler::end();
            return;
        }

        // Create new tag
        tagID = (uint16_t)(tagsIndex++);
        tags[tagID] = (char*)tag;

        // Remember tag type used
        tagTypes[tagID] = type;

        // Tag IDs start at 1
        tagID++;

        // Save it in the tag group when it is empty
        if (group != NULL && group->tagStart == 0)
            group->tagStart = tagID;
    }

    // Check if type actually matches the saved tag type
    DART_ASSERT(tagTypes[tagID - 1] == type, DART_ERROR_INVALID_VALUE);

    // Write the data itself
    bufferTag(tagID, data, size);
    Profiler::end();
}


void DataStream::log(TagGroup* group, const char* tag, bool value) {
    uint8_t val = value ? 1 : 0;
    log(group, tag, (uint8_t*)&val, sizeof(val), TagUint8);
}

void DataStream::log(TagGroup* group, const char* tag, uint16_t value){
    log(group, tag, (uint8_t*)&value, sizeof(value), TagUint16);
}

void DataStream::log(TagGroup* group, const char* tag, uint32_t value) {
    log(group, tag, (uint8_t*)&value, sizeof(value), TagUint32);
}

void DataStream::log(TagGroup* group, const char* tag, uint8_t value) {
    log(group, tag, (uint8_t*)&value, sizeof(value), TagUint8);
}

void DataStream::log(TagGroup* group, const char* tag, int8_t value) {
    log(group, tag, (uint8_t*)&value, sizeof(value), TagInt8);
}

void DataStream::log(TagGroup* group, const char* tag, int16_t value) {
    log(group, tag, (uint8_t*)&value, sizeof(value), TagInt16);
}

void DataStream::log(TagGroup* group, const char* tag, int32_t value) {
    log(group, tag, (uint8_t*)&value, sizeof(value), TagInt32);
}

void DataStream::log(TagGroup* group, const char* tag, float value) {
    log(group, tag, (uint8_t*)&value, sizeof(value), TagFloat);
}
#endif
