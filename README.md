# DSA
This repository contains the two projects made on the course Data Structures and Algorithms. 
The first project creates a program where information about affiliations and their publications can be entered and retrieved. 
The publications can refer to each other, which links them together. 
The second project expanses the first by implementing a graph, which connects the affiliations based on shared publications. 

Please note that the projects were made with a given template. Thus, the only part I have modified in each of the .hh -files is the private interface. 

Terminology:
Affiliations - affiliations are organizations such as universities or companies.
Publication - publications are scientific works produced by affiliations. Publications refer to other publications, forming a network of references.
Reference - references link publications to each other. 
Each publication can refer to an arbitrary number of other publications, but a publication can only be the source of one publication.
Connection - a connection between two affiliations with a distance and weight. 
