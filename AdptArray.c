#include "AdptArray.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct AdptArray_
{
	int ArrSize;
	PElement* pElemArr;
	DEL_FUNC delFunc;
	COPY_FUNC copyFunc;
    PRINT_FUNC printFunc;
}AdptArray;

typedef void* PElement;
typedef void(*DEL_FUNC)(PElement);
typedef PElement(*COPY_FUNC)(PElement);
typedef void(*PRINT_FUNC)(PElement);

PAdptArray CreateAdptArray(COPY_FUNC copyFunc_, DEL_FUNC delFunc_,PRINT_FUNC printFunc_){
    PAdptArray pArr = (PAdptArray)malloc(sizeof(AdptArray));
	if (pArr == NULL)
		return NULL;
	pArr->ArrSize = 0;
	pArr->pElemArr = NULL;
	pArr->delFunc = delFunc_;
	pArr->copyFunc = copyFunc_;
    pArr->printFunc = printFunc_;
	return pArr;
}
void DeleteAdptArray(PAdptArray pArr){
    int i;
	if (pArr == NULL)
		return;
	for(i = 0; i < pArr->ArrSize; ++i){
        if((pArr->pElemArr)[i] != NULL){
		pArr->delFunc((pArr->pElemArr)[i]);
        }
	}
	free(pArr->pElemArr);
	free(pArr);
}

Result SetAdptArrayAt(PAdptArray pArr, int index, PElement pNewElem){
PElement* newpElemArr; //the new pElemArr
	if (pArr == NULL)
		return FAIL;

	if (index >= pArr->ArrSize)
	{
    // Given index is greater then ArrSize so we need to extend the array
		if ((newpElemArr = (PElement*)calloc((index + 1), sizeof(PElement))) == NULL)
			return FAIL;
		memcpy(newpElemArr, pArr->pElemArr, (pArr->ArrSize) * sizeof(PElement));
		free(pArr->pElemArr);
		pArr->pElemArr = newpElemArr;
	}

	// delete prev element from the array at given index
    if((pArr->pElemArr)[index] != NULL){
        pArr->delFunc((pArr->pElemArr)[index]);
    }
	(pArr->pElemArr)[index] = pArr->copyFunc(pNewElem);

	// if index is gt arrsize , increase arrsize by 1
	pArr->ArrSize = (index >= pArr->ArrSize) ? (index + 1) : pArr->ArrSize;
	return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray pArr, int index){
    if(pArr == NULL)
        return NULL;
    if((pArr->pElemArr)[index] == NULL)
        return NULL;
    if(index < 0 || index >= pArr->ArrSize)
        return NULL;
    
    return pArr->copyFunc((pArr->pElemArr)[index]);
}

int GetAdptArraySize(PAdptArray pArr){
    if (pArr == NULL)
        return -1;

    return pArr->ArrSize;
}

void PrintDB(PAdptArray pArr){
    if (pArr == NULL)
        return;
    int i;
    for (i = 0; i < pArr->ArrSize; i++){
        if((pArr->pElemArr)[i] != NULL){
        pArr->printFunc((pArr->pElemArr)[i]);
        }
    }
}