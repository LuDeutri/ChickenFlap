#pragma once

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

// When the platform does not have a real 8 bit type, the data stream class is highly broken
#ifndef DEVICE_HAS_FAKE_INT8

#include "../../../chickenFlap/dart/highlevel/Log.h"
#include "../../../chickenFlap/dart/highlevel/Profiler.h"
#include "../../../chickenFlap/dart/highlevel/TagGroup.h"

/**
 * Maximum number of tags that are supported by the buffer.
 */
#define DATA_STREAM_MAX_TAG_COUNT 2048

/**
 * Type of the tag written to the log stream.
 */
enum TagType {
    TagUint8 = 1,
    TagUint16 = 2,
    TagUint32 = 3,
    TagInt8 = 4,
    TagInt16 = 5,
    TagInt32 = 6,
    TagFloat = 7
};

class DataStream {
private:
    /**
     * If the logging to the data stream is enabled.
     */
    bool enabled;

    /**
     * Size of the buffer.
     */
    size_t bufferSize;

    /**
	 * Buffer for the data written to the stream.
	 */
    uint8_t* dataBuffer;

    /**
     * Current index in the buffer.
     */
    size_t dataBufferIndex;

    /**
     * Current index of the last tag saved in tags.
     */
    int tagsIndex;

    /**
     * Array which holds all tags that are known to the logger.
     */
    char* tags[DATA_STREAM_MAX_TAG_COUNT];

    /**
    * Array which keeps track of the types of all tags.
    */
    TagType tagTypes[DATA_STREAM_MAX_TAG_COUNT];

    /**
	 * Finds the tag ID for the specified tag name.
	 */
    uint16_t findTag(TagGroup* group, const char* tag);

    /**
     * Writes the new tag structure to the data buffer.
     */
    bool bufferNewTag(uint16_t tagID, TagType type, const char* tagName);
    /**
     * Writes the tag structure to the data buffer.
     */
    void bufferTag(uint16_t tagID, uint8_t* data, size_t size);

public:
    explicit DataStream();
    explicit DataStream(size_t bufferSize);

    /**
     * Returns if the data stream is enabled (i.e. the log commands do anything)
     * @return True when the data stream is enabled and data is logged, false otherwise.
     */
    bool isEnabled();

    /**
     * Enables or disables the DataStream (i.e. the log commands do anything)
     * @param enabled True when the DataStream should be enabled or false otherwise.
     */
    void setEnabled(bool enabled);

    /**
     * Resets the DataStream, clearing any data.
     */
    void reset();

    /**
     * Returns the count of bytes which are not yet flushed.
     * @return The count of bytes which are not yet flushed.
     */
    size_t getDataPendingSize();

    /**
     * Returns the pointer to the internal buffer.
     * @return The pointer to the internal buffer.
     */
    uint8_t* getData();

    /**
     * Should be called when data was written from the data stream to any other source, i.e. any file.
     * The DataStream will then flush n bytes.
     * @param bytesWritten The count of bytes to flush from the buffer.
     */
    void flushData(size_t bytesWritten);

    /**
	 * Writes a tag to the data stream using the specified tag name, the given data with the given size and the given tag type.
	 */
    void log(TagGroup* group, const char* tag, uint8_t* data, size_t size, TagType type);
    /**
     * Writes a bool tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, bool value);
    /**
     * Writes a uint8_t tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, uint8_t value);
    /**
     * Writes a uint16_t tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, uint16_t value);
    /**
     * Writes a uint32_t tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, uint32_t value);
    /**
     * Writes a int8_t tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, int8_t value);
    /**
     * Writes a int16_t tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, int16_t value);
    /**
     * Writes a int32_t tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, int32_t value);
    /**
     * Writes a float tag to the data stream.
     */
    void log(TagGroup* group, const char* tag, float value);

    /**
	 * Initializes the tag group.
	 */
    static void initTagGroup(TagGroup* group, util_time_t interval);

    /**
     * Returns true when the given TagGroup should log data.
     */
    static bool shouldLog(TagGroup* group);
};
#endif
