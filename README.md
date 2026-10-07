# Spell Checker

## Purpose

The Spell Checker gets as input `dictionary` and `text of words`. It must find `dictionary` words that are no more than two `edits` away from a misspelt input word.

Here, an `edit` is:
- Single letter insertion
- Single letter deletion

Additional Restriction:
- If the edits are both insertions or both deletions, they may not be of adjacent characters.

## Input

The input will consist of a `dictionary` followed by a sequence of text lines containing possibly `misspelt words`. After the `dictionary` there is a line containing just the string `===`, then comes the text, followed again by a line containing just the string `===`. These `===` lines denote the end of each section:

```txt
  dictionary
  ===
  text lines
  ===
```

- The `dictionary` and the `text` contain words (strings of letters) of up to 50 characters.
- The dictionary is in free format, meaning dictionary words may be separated by arbitrary whitespace, including spaces, tabs, and newlines.
- The `misspelt words`
- The input is case-insensitive. Comparisons with the dictionary are case-insensitive.


Real input example:
```txt
rain spain plain plaint pain main mainly
the in on fall falls his was
===
hte rame in pain fells
mainy    oon teh lain
was hints pliant
===
```

## Output

- Print corrections from the dictionary in the case they appear in the
dictionary.
- Print unchanged words from the text lines in their original case.

As output, you should print the text lines with **whitespace intact**, with the following changes on each word, `W`:
- If `W` is in the `dictionary`, print it as is.
- If `W` is **not** in the `dictionary`:

  - Find all possible corrections that are no more than two edits away.
  - If there is at least one correction that requires **one edit**, ignore all corrections that require **two edits**.
  - If no corrections can be found, print `"{W?}"`.
  - If exactly one correction remains, print that word.
  - If more than one correction remains, print the set of corrections as `"{W1 W2 ···}"`, in the order they appear in the dictionary.


Real output example (for the input example above):

```txt
the {rame?} in pain falls
{main mainly}    on the plain
was {hints?} plaint
```