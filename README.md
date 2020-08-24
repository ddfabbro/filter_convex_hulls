# filter_convex_hulls

Eliminate overlapped convex hulls

- Load `convex_hulls.json`.

- Some of the convex hulls stored in `convex_hulls.json` are overlapped.

- Eliminate the overlapped convex hulls based on the following rule.
  - Eliminate if the convex hull has more than 50% its own area overlapped with
    other convex hulls.
    
- Generate `result_convex_hulls.json` which stores only the remaining (non-eliminated)
  convex hulls. The format should be the same as `convex_hulls.json`.
  
- Authorized program language is only C++.
  - Follow a coding style guide which you are using.
    - E.g. Google C++ Style Guide
  - Mention the style guide as a comment on the top of your program code.
    - `// This code follows Google C++ Style Guide`.

- You are allowed to use JSON library to read/write a json file.
  - E.g. https://nlohmann.github.io/json/.

- You are allowed to use the std library in C++.

- Do not use any other libraries, especially to calculate area or intersection.

- Put appropriate comments for the reviewer to understand your code easily.

- Submit both your code and your `result_convex_hulls.json` file.
