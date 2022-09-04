//
// Created by Michael Sendker on 9/1/22.
//

#include "Markdown.h"
#include <iostream>
#include <sstream>
#include <cstring>

/*
 * MD4C: Markdown parser for C
 * (http://github.com/mity/md4c)
 *
 * Copyright (c) 2016-2020 Martin Mitas
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/*
 * Heavily modified by Michael Sendker on 9/2/22.
 */

#include "Constants.h"

#include "md4c-html.h"



/* Global options. */
static unsigned parser_flags = 0;
static unsigned renderer_flags = Constants::MD_FLAGS;


/*********************************
 ***  Simple grow-able buffer  ***
 *********************************/

/* We render to a memory buffer instead of directly outputting the rendered
 * documents, as this allows using this utility for evaluating performance
 * of MD4C (--stat option). This allows us to measure just time of the parser,
 * without the I/O.
 */

struct membuffer {
    char* data;
    size_t asize;
    size_t size;
};

static void
membuf_init(struct membuffer* buf, MD_SIZE new_asize)
{
    buf->size = 0;
    buf->asize = new_asize;
    buf->data = (char*)malloc(buf->asize);
    if(buf->data == nullptr) {
        fprintf(stderr, "membuf_init: malloc() failed.\n");
        exit(1);
    }
}

static void
membuf_fini(struct membuffer* buf)
{
    if(buf->data)
        free(buf->data);
}

static void
membuf_grow(struct membuffer* buf, size_t new_asize)
{
    buf->data = (char*)realloc(buf->data, new_asize);
    if(buf->data == nullptr) {
        fprintf(stderr, "membuf_grow: realloc() failed.\n");
        exit(1);
    }
    buf->asize = new_asize;
}

static void
membuf_append(struct membuffer* buf, const char* data, MD_SIZE size)
{
    if(buf->asize < buf->size + size)
        membuf_grow(buf, buf->size + buf->size / 2 + size);
    memcpy(buf->data + buf->size, data, size);
    buf->size += size;
}


/**********************
 ***  Main program  ***
 **********************/

static void
process_output(const MD_CHAR* text, MD_SIZE size, void* userdata)
{
    membuf_append((struct membuffer*) userdata, text, size);
}


std::string Markdown::parse(const std::string &raw)
{
    struct membuffer buf_in = {0};
    struct membuffer buf_out = {0};

    membuf_init(&buf_in, 32 * 1024);

    /* Read the input file into a buffer. */
    // convert raw to a char *
    strcpy(buf_in.data, raw.c_str());
    buf_in.size = raw.size() + 1;
    buf_in.asize = raw.size() + 1;

    /* Input size is good estimation of output size. Add some more reserve to
     * deal with the HTML header/footer and tags. */
    membuf_init(&buf_out, buf_in.size + buf_in.size/8 + 64);

    int ret = md_html(buf_in.data, buf_in.size, process_output, (void*) &buf_out,
                  parser_flags, renderer_flags);

    if(ret != 0) {
        std::cerr << "Parsing failed.\n" << std::endl;
        exit(1);
    }

    // convert buf_out.data to a std::string
    buf_out.data[buf_out.size] = '\0';
    std::string output(buf_out.data);

    membuf_fini(&buf_in);
    membuf_fini(&buf_out);
    return output;
}

std::unordered_map<std::string, std::string> Markdown::parse_frontmatter(const std::string &raw)
{
    std::unordered_map<std::string, std::string> frontmatter;
    size_t start = raw.find("---\n") + 4;
    size_t end = raw.find("---", start);
    std::string frontmatter_raw = raw.substr(start, end - start);
    std::istringstream iss(frontmatter_raw);
    std::string line;
    while (std::getline(iss, line)) {
        size_t colon = line.find(':');
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);
        // trim value
        value.erase(0, value.find_first_not_of(' '));
        value.erase(value.find_last_not_of(' ') + 1);
        frontmatter[key] = value;
    }
    return frontmatter;
}

size_t Markdown::get_frontmatter_end(const std::string &line)
{
    return line.find("---", 4) + 4; // 4 is the length of "---\n"
}
