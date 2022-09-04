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

That's cool and all, and I use it, but its JavaScript (🤮😜). No, but 
seriously speaking, I've had a simple PR open for Sergey for 2 years now (https://github.com/trys/sergey/pull/52)
with no response from Trys so I'm not interested in relying on it anymore, 
and I figure something like this is simple enough that I can learn more C++ 
by writing it.

Also, I feel like I'm already making some advancements, like arbitrary file 
location. And, you know, my dog died, and I wanted to make something named 
after her. Pic coming soon.

So, this is for you, Sky Blue.

# TODO

- [X] Change constants class to a namespace
- [X] Templates
- [X] Markdown
- [ ] Tests
- [X] Pipeline
- [ ] Development server mode