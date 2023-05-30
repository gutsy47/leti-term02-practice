# Course work
## C++ AVL Tree

Details provided in the *report.docx*.

### Notice
The command 3 (Generating based on `input.txt` tests) uses the `generateAnswers()` function, 
which uses environment variables.
```C++
113. void generateAnswers() {
114.    // Environment variables
115.    const char* INPUT_PATH = std::getenv("INPUT_PATH");
116.    const char* OUTPUT_KEY_PATH = std::getenv("OUTPUT_KEY_PATH");
117.    const char* OUTPUT_ANS_PATH = std::getenv("OUTPUT_ANS_PATH");
...
```
If you are going to read instructions from a file, 
**make sure that environment variables have been specified**

| Variable        | Value                |
|-----------------|----------------------|
| INPUT_PATH      | `...\input.txt`      |
| OUTPUT_KEY_PATH | `...\output_key.txt` |
| OUTPUT_ANS_PATH | `...\output.ans.txt` |
