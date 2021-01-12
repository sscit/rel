# HTML Export Script

Python3 Script [html.py](./html.py) uses REL Python3 module, to read the [REL requirements](../../requirements) and create a HTML document out of them.

The HTML code is printed to the console.

## Run Script with Bazel

```
bazel run //relpy/html_export:html > rel_requirements.html
```
