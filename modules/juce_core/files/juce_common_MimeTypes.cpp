/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2022 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 7 End-User License
   Agreement and JUCE Privacy Policy.

   End User License Agreement: www.juce.com/juce-7-licence
   Privacy Policy: www.juce.com/juce-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

struct MimeTypeTableEntry
{
    String fileExtension, mimeType;

    bool operator< (const struct MimeTypeTableEntry& o) const
    {
        return std::array<String, 2>({{fileExtension,     mimeType}})
             < std::array<String, 2>({{o.fileExtension, o.mimeType}});
    }

    static std::set<MimeTypeTableEntry>& get();
};

static StringArray getMatches (const String& toMatch,
                               String MimeTypeTableEntry::* matchField,
                               String MimeTypeTableEntry::* returnField)
{
    StringArray result;

    for (auto type : MimeTypeTableEntry::get())
        if (toMatch == type.*matchField)
            result.add (type.*returnField);

    return result;
}

namespace MimeTypeTable
{

StringArray getMimeTypesForFileExtension (const String& fileExtension)
{
    return getMatches (fileExtension, &MimeTypeTableEntry::fileExtension, &MimeTypeTableEntry::mimeType);
}

StringArray getFileExtensionsForMimeType (const String& mimeType)
{
    return getMatches (mimeType, &MimeTypeTableEntry::mimeType, &MimeTypeTableEntry::fileExtension);
}

void registerCustomMimeTypeForFileExtension (const String& mimeType, const String& fileExtension)
{
    MimeTypeTableEntry pair {fileExtension, mimeType};
    auto& table = MimeTypeTableEntry::get();

    if (auto it = table.find (pair); it != table.end())
        table.erase (it);

    table.emplace (std::move (pair));
}

} // namespace MimeTypeTable

//==============================================================================
std::set<MimeTypeTableEntry>& MimeTypeTableEntry::get()
{
	static std::set<MimeTypeTableEntry> result;
    return result;
}

} // namespace juce
