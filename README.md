# fer-bioinf

# TASK
## (5) Faza konsenzusa u OLC paradigmi sastavljanja genoma – Sparc (MŠ)

* Ulaz: kontizi u FASTA formatu (85% točnosti kada se usporede s referencom), skup očitanja u FASTQ formatu, skup mapiranja očitanja na kontige (MHAP ili PAF format, trebate generirati sami pomoću minimap-a (https://github.com/lh3/minimap) ili GraphMap-a (https://github.com/isovic/graphmap ili oba zasebno za bolju evaluaciju vaših rješenja).
* Cilj: Pomoću skupa početnih očitanja povećati točnost kontiga dobivenih iz faze razmještaja.
* Izlaz: popravljeni kontizi u FASTA formatu.
* Evaluacija: koristiti alat DnaDiff iz mummer paketa (sudo apt-get install mummer). Za testne skupove javiti se nastavniku.


1. Implementirati sparse k-mer graph (Sparc: a sparsity-based consensus algorithm for long erroneous sequencing reads (http://tinyurl.com/zqjcd9q).)
2. Implementirati konsenzus modul pomoću 1. dijela.

## [Testni podatci](https://www.dropbox.com/s/a40dhhfchojyf0c/consenus_input.zip?dl=0)

# INSTRUCTIONS
## TODO

# RESULTS
## Specifications
| OS | PROCESSOR | RAM |
| -- | --------- | --- |
| Arch Linux x86_64 | Intel Core i3-6100 @ 3.70GHz | 16 GiB DDR4 @ 2133MHz |

## After 1 iteration
| Instance Name | Base Score | Our Score | Time   | Memory | Improvement | Original implementation Score | Time  | Memory  | Improvement |
| ------------- | ---------- | --------- | ------ | ------ | ----------  | -------------- | ----  | ------  | ----------- |
| Lambda        | 86.16      | 90.94     | 0.14s  | 20MiB  | 5.5 %       | 96.83          | 0.27s | 32MiB   | 12.4%       |
| Ecoli         | 88.58      | 94.80     | 26.8s  | 1.5GiB | 7.02 %      | 99.01          | 36.8s | 3GiB  | 11.77%      |

## After 2 iterations
| Instance Name | Base Score | Our Score | Improvement |
| ------------- | ---------- | --------- | ----------- |
| Lambda        | 86.16      | 91.54     | 6.2 %       |
| Ecoli         | 88.58      | 95.87     | 8.2 %       |

No further progress could be found.
