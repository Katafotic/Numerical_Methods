#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep 17 00:42:02 2020

@author: viacheslav
"""

import numpy as np
dimention = 11
P = np.array([1.0000000, 0.0156250, 0.0000000], dtype=float)

filename = '/Experiences/Numerical_Methods/Lab1/RGEmtr029.dat'
def get_matrix(filename, dimention) -> float:
    with open(filename, 'r') as input:
        matrix = np.empty([dimention, dimention], dtype=float)
        for line in range(dimention):
            matrix[line] = np.fromfile(input, dtype=float, count=dimention, sep=' ')
        return matrix

A = get_matrix(filename, dimention)
print(A)