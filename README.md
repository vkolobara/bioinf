# Sparc algorithm: consensus phase in Overlap-Layout-Consensus genome assembly
This project was done as a part of course Bioinformatics at Faculty of Electrical Engineering and Computing, Zagreb.
Course description and details can be found at http://www.fer.unizg.hr/predmet/bio.

## TASK
Implement and evaluate OLC genome assembly, consensus phase algorithm Sparc. Reference white paper can be found here http://tinyurl.com/zqjcd9q. Detailed project instructions can be found in this repository.

### [Test Data](https://www.dropbox.com/s/a40dhhfchojyf0c/consenus_input.zip?dl=0)

## INSTRUCTIONS
### TODO

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
