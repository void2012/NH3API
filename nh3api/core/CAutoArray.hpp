//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std/memory.hpp"

template <class T> 
class CAutoArray 
{
 protected:
 public:
  CAutoArray() 
  {
    step = 25;
    size = 0;
    allocSize = 0;
    pArray = nullptr;
  }

  //virtual ~CAutoArray() 
  //{ Destroy(); }
  NH3API_SCALAR_DELETING_DESTRUCTOR

  void Destroy(bool deleteData = true) 
  {
    if (deleteData) 
    {
      for (uint32_t i = 0; i < size; i++) 
      {
        T* pElement = Get(i);
        exe_invoke_delete(pElement);
      }
    }

    if (pArray) 
        exe_delete(pArray);

    pArray = nullptr;
    allocSize = 0;
    size = 0;
  }

  void SetStep(uint32_t newStep) 
  { step = newStep; }

  virtual bool Allocate(uint32_t newSize) 
  {
    allocSize = newSize;

    uint32_t* pTemp = new (exe_heap) uint32_t[allocSize];

    if (pArray) 
    {
      memcpy(pTemp, pArray, sizeof(uint32_t) * size);
      exe_delete(pArray);
    }
    pArray = pTemp;

    return true;
  }

  virtual bool Add(T* element) 
  {
    if (allocSize <= size) 
    {
      // time to realloc
      Allocate(allocSize + step);
    }
    size++;
    return Put(size - 1, element);
  }

  virtual T* Get(uint32_t elementNbr) 
  {
    if (elementNbr >= size) 
      return nullptr;
    return reinterpret_cast<T*>(pArray[elementNbr]);
  }

  virtual bool Put(uint32_t elementNbr, T* element) 
  {
    if (elementNbr >= size) return false;
    pArray[elementNbr] = reinterpret_cast<uint32_t>(element);
    return true;
  }

  virtual bool Delete(uint32_t elementNbr) 
  {
    if (elementNbr >= size) return false;

    for (uint32_t index = elementNbr; index < size - 1; index++) {
      pArray[index] = pArray[index + 1];
    }
    size--;

    return true;
  }

  virtual bool Insert(uint32_t nextElementNbr, T* element) 
  {
    if (nextElementNbr >= size) 
      return false;

    T* save = Get(size - 1);

    for (uint32_t index = size - 1; index > nextElementNbr; index--) 
    {
      pArray[index] = pArray[index - 1];
    }

    Put(nextElementNbr, element);
    Add(save);

    return true;
  }

  virtual uint32_t GetCount() 
  { return size; }

  T* operator[](uint32_t index) 
  { return Get(index); }

 protected:
  uint32_t step;
  uint32_t* pArray;
  uint32_t allocSize;
  uint32_t size;
};