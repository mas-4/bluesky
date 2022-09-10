//
// Created by mas on 7/4/22.
//

#ifndef BLUESKY_CONSTANTS_H
#define BLUESKY_CONSTANTS_H

#include <string>
#include <unordered_map>
#include "md4c.h"

namespace Constants
{

    enum ImportType
    {
        IT_INCLUDE,
        IT_TEMPLATE,
        IT_MARKDOWN,
        IT_SLOT,
        IT_BLOCK,
        IT_BLOCK_END,
        IT_UNKNOWN,
        IT_CODE
    };
    static const unsigned MD_FLAGS = MD_FLAG_COLLAPSEWHITESPACE | MD_FLAG_TABLES | MD_FLAG_TASKLISTS |
                                     MD_FLAG_STRIKETHROUGH | MD_FLAG_PERMISSIVEAUTOLINKS |
                                     MD_FLAG_PERMISSIVEWWWAUTOLINKS |
                                     MD_FLAG_LATEXMATHSPANS;
    static const size_t IMPORT_TAG_LENGTH = 7;
    static const std::string OPENER = "<bluesky-";
    static const std::string CLOSER = ">";
    static const std::string VARIABLE_OPENER = "${";
    static const std::string VARIABLE_CLOSER = "}";
    static const std::string IMPORT_TAGS[IMPORT_TAG_LENGTH] = {
            "<bluesky-include",
            "<bluesky-template",
            "<bluesky-markdown",
            "<bluesky-slot",
            "<bluesky-block",
            "</bluesky-block>",
            "```",
    };
    static const std::string TEMPLATE_END = "</bluesky-template>";
    static const size_t DEFAULT_PORT = 8080;
    static const std::unordered_map<std::string, std::string> MIME_TYPES = {
            {".html",  "text/html"},
            {".css",   "text/css"},
            {".js",    "application/javascript"},
            {".png",   "image/png"},
            {".jpg",   "image/jpeg"},
            {".jpeg",  "image/jpeg"},
            {".gif",   "image/gif"},
            {".svg",   "image/svg+xml"},
            {".ico",   "image/x-icon"},
            {".json",  "application/json"},
            {".pdf",   "application/pdf"},
            {".zip",   "application/zip"},
            {".gz",    "application/gzip"},
            {".txt",   "text/plain"},
            {".md",    "text/markdown"},
            {".xml",   "text/xml"},
            {".xhtml", "application/xhtml+xml"},
            {".wasm",  "application/wasm"},
            {".webp",  "image/webp"},
            {".webm",  "video/webm"},
            {".mp4",   "video/mp4"},
            {".mp3",   "audio/mpeg"},
            {".ogg",   "audio/ogg"},
            {".wav",   "audio/wav"},
            {".flac",  "audio/flac"},
            {".aac",   "audio/aac"},
            {".m4a",   "audio/mp4"},
            {".m4v",   "video/mp4"},
            {".mov",   "video/quicktime"},
            {".avi",   "video/x-msvideo"},
            {".wmv",   "video/x-ms-wmv"},
            {".flv",   "video/x-flv"},
            {".mkv",   "video/x-matroska"},
            {".ts",    "video/mp2t"},
            {".3gp",   "video/3gpp"},
            {".3g2",   "video/3gpp2"},
            {".7z",    "application/x-7z-compressed"},
            {".rar",   "application/x-rar-compressed"},
            {".tar",   "application/x-tar"},
            {".bz2",   "application/x-bzip2"},
            {".doc",   "application/msword"},
            {".docx",  "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    };

}

#endif //BLUESKY_CONSTANTS_H
