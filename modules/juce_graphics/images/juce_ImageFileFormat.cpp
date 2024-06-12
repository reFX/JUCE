/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

struct DefaultImageFormats
{
    static std::vector<std::unique_ptr<ImageFileFormat>>& get()
    {
        static DefaultImageFormats formats;
        return formats.formats;
    }

private:
    DefaultImageFormats() noexcept
    {
        formats.push_back (std::make_unique<PNGImageFormat>());
        formats.push_back (std::make_unique<JPEGImageFormat>());
        formats.push_back (std::make_unique<GIFImageFormat>());
    }

    std::vector<std::unique_ptr<ImageFileFormat>> formats;
};

void ImageFileFormat::registerFileFormat (std::unique_ptr<ImageFileFormat> format)
{
    DefaultImageFormats::get().push_back (std::move (format));
}

ImageFileFormat* ImageFileFormat::findImageFormatForStream (InputStream& input)
{
    const int64 streamPos = input.getPosition();

    for (auto& format : DefaultImageFormats::get())
    {
        const bool found = format->canUnderstand (input);
        input.setPosition (streamPos);

        if (found)
            return format.get();
    }

    return nullptr;
}

ImageFileFormat* ImageFileFormat::findImageFormatForFileExtension (const File& file)
{
    for (auto& format : DefaultImageFormats::get())
        if (format->usesFileExtension (file))
            return format.get();

    return nullptr;
}

//==============================================================================
Image ImageFileFormat::loadFrom (InputStream& input)
{
    if (ImageFileFormat* format = findImageFormatForStream (input))
        return format->decodeImage (input);

    return Image();
}

Image ImageFileFormat::loadFrom (const File& file)
{
    FileInputStream stream (file);

    if (stream.openedOk())
    {
        BufferedInputStream b (stream, 8192);
        return loadFrom (b);
    }

    return Image();
}

Image ImageFileFormat::loadFrom (const void* rawData, const size_t numBytes)
{
    if (rawData != nullptr && numBytes > 4)
    {
        MemoryInputStream stream (rawData, numBytes, false);
        return loadFrom (stream);
    }

    return Image();
}

} // namespace juce
