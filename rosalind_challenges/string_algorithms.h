#pragma once
#include "general_functions.h"

int counting_DNA_nucleotides(char *argv[]);
void count_nucleotides();
int transcribing_DNA_into_RNA(char *argv[]);
void make_RNA();
int complementing_a_strand_of_DNA(char *argv[]);
void complement_DNA();
int computing_GC_content(char *argv[]);
void count_GC_content(int *counts, char *line);
int translating_RNA_into_protein(char *argv[]);
char *RNA_to_protein(char *rna_string);
int finding_a_motif_in_DNA(char *argv[]);
int RNA_splicing(char *argv[]);
char *DNA_to_RNA(char *DNA);
int finding_a_spliced_motif(char *argv[]);
int locating_restriction_sites(char *argv[]);
int open_reading_frames(char *argv[]);