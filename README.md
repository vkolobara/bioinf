# fer-bioinf

## (5) Faza konsenzusa u OLC paradigmi sastavljanja genoma – Sparc (MŠ)

* Ulaz: kontizi u FASTA formatu (85% točnosti kada se usporede s referencom), skup očitanja u FASTQ formatu, skup mapiranja očitanja na kontige (MHAP ili PAF format, trebate generirati sami pomoću minimap-a (https://github.com/lh3/minimap) ili GraphMap-a (https://github.com/isovic/graphmap ili oba zasebno za bolju evaluaciju vaših rješenja).
* Cilj: Pomoću skupa početnih očitanja povećati točnost kontiga dobivenih iz faze razmještaja.
* Izlaz: popravljeni kontizi u FASTA formatu.
* Evaluacija: koristiti alat DnaDiff iz mummer paketa (sudo apt-get install mummer). Za testne skupove javiti se nastavniku.


1. Implementirati sparse k-mer graph (Sparc: a sparsity-based consensus algorithm for long erroneous sequencing reads (http://tinyurl.com/zqjcd9q).)
2. Implementirati konsenzus modul pomoću 1. dijela.
