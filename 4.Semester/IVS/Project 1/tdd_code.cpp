//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Vojtech Fiala <xfiala61@stud.fit.vutbr.cz>
// $Date:       $2021-03-03
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Vojtech Fiala
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

PriorityQueue::PriorityQueue()
{
    m_pHead = NULL;

}

PriorityQueue::~PriorityQueue()
{
    auto *ptr = m_pHead;
    while (ptr != NULL) {
        auto *del = ptr;
        ptr = ptr->pNext;
        delete del;
    }
}

void PriorityQueue::Insert(int value)
{
    auto to_insert = new Element_t();
    to_insert->value = value;
    to_insert->pNext = NULL;

    // if the queue is empty, imma insert it as the new root
    if (m_pHead == NULL) {
        m_pHead = to_insert;
    }
    // if it aint empty

    else {
        auto position = m_pHead->value;
        // if its higher or same as the root, its gonna be my new root
        if (value >= position) {
            auto *save = m_pHead;
            auto *item = m_pHead->pNext;
            to_insert->pNext = save;
            m_pHead = to_insert;
            save->pNext = item;
            
        }
        // if its lower, find a new spot for it
        else {
            auto *next = m_pHead;
            while (value < next->pNext->value) {
                next = next->pNext;

                //check if im not at the end
                if (next->pNext == NULL) 
                    break;
            }

            // move it to its new position
                auto *link = next->pNext;
                to_insert->pNext = link;
                next->pNext = to_insert;
        }
    }
}

bool PriorityQueue::Remove(int value)
{
    // if im removing the head
    if (m_pHead == NULL) {
        return false;
    }

    if (value == m_pHead->value) {
        auto to_rem = m_pHead;
        m_pHead = m_pHead->pNext;
        delete to_rem;
        return true;
    }
    else if (value < m_pHead->value) {
        auto *next = m_pHead->pNext;
        auto *prev = m_pHead->pNext;
        if (next != NULL) {
            if (next->value == value) {
                auto del = next;
                m_pHead->pNext = next->pNext;
                delete del;
                return true;
            }
        }
        while (next != NULL) {
            // check if im not at the end
            if (next->pNext != NULL) {
                if (value == next->pNext->value) {
                    auto del = next->pNext;
                    next->pNext = next->pNext->pNext;
                    delete del;
                    return true;
                }
            }
            else {
                break;
            }
            prev = next;
            next = next->pNext;
        }
    }
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    auto it = m_pHead;
    if (it != NULL) {

        while (it->value != value) {
            it = it->pNext;
            if (it == NULL)
                break;
        }
    }

    return it;
}

size_t PriorityQueue::Length()
{
    auto it = m_pHead;
    int counter = 0;
    while (it != NULL) {
        counter++;
        it = it->pNext;
    }
	return counter;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
