# Markov-Chain Tweet Generator (C)

This project implements a **probabilistic text generator** using a **Markov chain** built from an input corpus.
It learns transition frequencies between words, then generates tweets by sampling the next word with probability
proportional to its frequency. The project emphasizes **systems programming**, **dynamic memory management**, and
**reproducible stochastic modeling**.

---

## 🚀 Features
- **Corpus ingestion & learning:** tokenizes text, builds a unique node for each word, and maintains a frequency list of successors.
- **Probabilistic generation:** selects next words using weighted random sampling (∝ frequency).
- **Deterministic runs:** uses `srand(seed)` — same seed → identical tweets (important for reproducibility).
- **Memory safety:** fully Valgrind-clean with proper allocation & deallocation.
- **Robust CLI interface:** accepts seed, number of tweets, file path, and optional limit on number of words to read.

---

## 🧩 How It Works
1. **Learning phase:**  
   - Parse the corpus, create `MarkovNode` for each word if not already present.  
   - Maintain a frequency list of possible successors, incrementing count for repeated appearances.

2. **Generation phase:**  
   - Choose a random **non-terminal word** as the starting node.  
   - Repeatedly sample a successor using cumulative weights until:  
     - A terminal word is generated (ends with `.`), or  
     - The maximum length (default 20 words) is reached.

3. **Output:**  
   - Each tweet is printed with a prefix:  
     ```
     Tweet 6: hello, nice to meet you.
     ```

---

## 🛠 Build
Compile with:
```bash
gcc -Wall -Wextra -Wvla -std=c99 tweets_generator.c markov_chain.c linked_list.c -o tweets_generator
