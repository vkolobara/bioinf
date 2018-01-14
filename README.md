# Sparc algorithm: consensus phase in Overlap-Layout-Consensus genome assembly
This project was done as a part of course Bioinformatics at Faculty of Electrical Engineering and Computing, Zagreb.
Course description and details can be found at http://www.fer.unizg.hr/predmet/bio.

## TASK
Implement and evaluate OLC genome assembly, consensus phase algorithm Sparc. Reference white paper can be found here http://tinyurl.com/zqjcd9q. Detailed project instructions can be found in this repository.

### [Test Data](https://www.dropbox.com/s/a40dhhfchojyf0c/consenus_input.zip?dl=0)

## INSTRUCTIONS
### Installation
#### Prerequisites
* Linux OS
* CMAKE > 3.5
* gcc > 4
* wget
* unzip

#### Download and install project
```shell
git clone https://github.com/vkolobara/bioinf.git
cd bioinf
./install.sh
```

The installation script will install all dependencies in the libs directory and build the project to the build directory.

---

### Running
#### Executing the program with run.sh
```shell
./run.sh 2 lambda
```

The command above will run 2 iterations of the algorithm (k=3, g=4) on the lambda dataset. Run.sh searches for data files in the data directory with the following naming:
* layout    -> data/$INSTANCE_layout.fasta
* reads     -> data/$INSTANCE_reads.fastq
* reference -> data/$INSTANCE_reference.fasta

Output file will be generated in the same fashion:
* consensus -> data/$INSTANCE_sparc_output.fasta
---

run.sh depends on multiple other scripts:

mapping.sh maps the reads onto the layout with graphmap
```shell
./mapping.sh layout_path reads_path output_path
```

print_score.sh displays the percentage of correct mapping with dnadiff
```shell
./print_score.sh data/$instance_sparc_output.fasta data/$instance_reference.fasta
```

---

#### Run the program directly
```shell
./build/bioinf k g layout_path mapping_path output_path
```

The program takes 5 arguments:
* k            -> size of the k-mer in graph
* g            -> skip size
* layout_path  -> path to the layout file (.fasta)
* mapping_path -> path to the mapping file (.sam) 
* output_path  -> output path (.fasta)
---
## RESULTS
### Specifications
| OS | PROCESSOR | RAM |
| -- | --------- | --- |
| Arch Linux x86_64 | Intel Core i3-6100 @ 3.70GHz | 16 GiB DDR4 @ 2133MHz |

### After 1 iteration
| Instance Name | Base Score | Our Score | Time   | Memory | Improvement | Original implementation Score | Time  | Memory  | Improvement |
| ------------- | ---------- | --------- | ------ | ------ | ----------  | -------------- | ----  | ------  | ----------- |
| Lambda        | 86.16      | 90.94     | 0.14s  | 20MiB  | 5.5 %       | 96.83          | 0.27s | 32MiB   | 12.4%       |
| Ecoli         | 88.58      | 94.80     | 26.8s  | 1.5GiB | 7.02 %      | 99.01          | 36.8s | 3GiB  | 11.77%      |

### After 2 iterations
| Instance Name | Base Score | Our Score | Improvement |
| ------------- | ---------- | --------- | ----------- |
| Lambda        | 86.16      | 91.54     | 6.2 %       |
| Ecoli         | 88.58      | 95.87     | 8.2 %       |

No further progress could be found.
