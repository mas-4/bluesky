# bluesky

I'm sick of ultra complex static site generators.

Especially when the functionality could be easily reproduced if html 
implemented these basic features:

1. HTML includes (just like #include in c/cpp, drop the included file 
   wholecloth into the including file)
2. HTML templates (the reverse of #include, drop the templated file directly 
   into the template, using named slots)
3. HTML markdown (create a template for each markdown file, generate a list 
   of links, and convert every file within a directory into an html page 
   using the template)

The closest thing to doing just that I've found is [Sergey](https://sergey.cool/)
which is a nice little project written as a single file of JavaScript.

That's cool and all, and I use it, but its JavaScript (🤮😜).

C++ is also gross, but way easier than C, which I do want to eventually convert this to. And, you know, my dog died, and I wanted to make something named after her. Pic coming soon.

So, this is for you, Sky Blue.

# Usage

To build a site:

```
bluesky -i ./src -o ./build
```

To use the development server, you still need the output directory, even if we never write to it (a limitation I 
will overcome soon):

```
bluesky -i ./src -o ./build -h 0.0.0.0
```


The host can be any ip with a colon port. The default port is 8080 if no port is provided.

`-v` is verbose mode, which I find useful.

# Syntax

```html
<bluesky-include name="_nav.html" />
```

Will automatically include `src/_imports/_nav.html` wholesale, and also parse it (i.e., `_nav.html` can include 
other includes). The underscore is unnecessary.

```html
<bluesky-template name="_imports/template.html" />
<bluesky-block name="content">
   Hello, world!
</bluesky-block>
```

Will render with this template:

```html
<DOCTYPE !html>
<body>
    <bluesky-slot name="content" />
</body>
```

into

```html
<DOCTYPE !html>
<body>
    Hello, world!
</body>
```

Given a file like this:

```html
<bluesky-template name="_imports/template.html" />
<bluesky-block name="content" />
    <bluesky-markdown
       slot="content"
       directory="posts"
       title="title"
       sort="date"
       order="descending"
       template="_imports/post.html"
    />
</bluesky-block>
```

The file in question will render with an unordered list of links, using the `title` variable from the frontmatter as 
the anchor text, the template as the template for the rendered markdown (assuming markdown files in the `directory`),
the `date` variable from the frontmatter as the sort key, in order `descending`, and sticking the rendered markdown 
in the slot `content` in the markdown template `_imports/post.html`.

Finally, you can use variables from a `meta` file in the root of `src` anywhere:

```
site_title=bluesky
```

```html
<h1>${meta.site_title}</h1>
```

will render

```html
<h1>bluesky</h1>
```

and in the template for markdown posts you can use the frontmatter variables like so:

```html
<h1>${frontmatter.title}</h1>
```

And that's all there is to it.
