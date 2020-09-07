# -*- coding: utf-8 -*-

def bubble_sort(A):
    """ сортировка списка А методом пузырька """
    N = len(A)
    for bypass in range(1, N) :
        for k in range(0, N-bypass) :
            if A[k] > A[k+1]:
                A[k], A[k+1] = A[k+1], A[k]

def choise_sort(A):
    """ сортировка списка А выбором """
    N = len(A)
    for pos in range(0, N-1):
        for k in range(pos+1, N):
            if A[k] < A[pos]:
                A[k], A[pos] = A[pos], A[k]

def insert_sort(A):
    """ сорировка списка А методом вставки """
    N = len(A)
    for top in range(1, N):
        k = top
        while k > 0 and A[k-1] > A[k]:
            A[k], A[k-1] = A[k-1], A[k]
            k -= 1



def sort_test(sort_method):
    """ Тестирование сортировки sort_method """
    print("Тестируем: ", sort_method.__doc__)
    print("testcase #1: ",end="")
    A = [1, 3, 2, 5, 4]
    A_sorted = [1, 2, 3, 4, 5]
    sort_method(A)
    print("Ok" if A == A_sorted else "Fail")

    print("Тестируем: ", sort_method.__doc__)
    print("testcase #2: ",end="")
    A = list(range(3, 7)) + list(range(0, 3)) + list(range(7, 13))
    A_sorted = list(range(13))
    sort_method(A)
    print("Ok" if A == A_sorted else "Fail")

    print("Тестируем: ", sort_method.__doc__)
    print("testcase #3: ",end="")
    A = [1, 1, 2, 3, 1, 4, 3]
    A_sorted = [1, 1, 1, 2, 3, 3, 4]
    sort_method(A)
    print("Ok" if A == A_sorted else "Fail")


if __name__ == "__main__" :
    sort_test(bubble_sort)
    sort_test(choise_sort)
    sort_test(insert_sort)
