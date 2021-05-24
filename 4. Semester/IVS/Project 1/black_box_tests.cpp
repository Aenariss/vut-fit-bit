//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Vojtech Fiala <xfiala61@stud.fit.vutbr.cz>
// $Date:       $2020-03-03
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Vojtech Fiala
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

#define ASSERT_NEQ ASSERT_NE    // much better
#define EXPECT_NEQ EXPECT_NE

class EmptyTree : public::testing::Test {

    protected:
        BinaryTree *Baum;

        void SetUp() {
            Baum = new BinaryTree();
        }

        void TearDown() {
            delete Baum;
        }
};

TEST_F(EmptyTree, InsertNode_001) {

    auto Item_1 = Baum->InsertNode(42); 
    ASSERT_EQ(Item_1.first, true);
    ASSERT_NEQ(Item_1.second, nullptr);

    ASSERT_EQ(Item_1.second->pParent, nullptr);
    ASSERT_EQ(Item_1.second->key, 42);


}

TEST_F(EmptyTree, DeleteNode_001) {

    auto result = Baum->DeleteNode(42);
    ASSERT_EQ(result, false);
}

TEST_F(EmptyTree, FindNode_001) {

    auto item = Baum->FindNode(42);
    ASSERT_EQ(item, nullptr);
}

class NonEmptyTree : public::testing::Test {

    protected:
        BinaryTree *Baum;

        void SetUp() {
            Baum = new BinaryTree();
        }

        void TearDown() {
            delete Baum;
        }
};

TEST_F(NonEmptyTree, InsertNode_NonEmpty_001) {

    auto Item_1 = Baum->InsertNode(42);
    ASSERT_EQ(Item_1.first, true);
    ASSERT_NEQ(Item_1.second, nullptr);
    ASSERT_EQ(Item_1.second->key, 42);

    auto Item_2 = Baum->InsertNode(43);
    EXPECT_EQ(Item_2.first, true);
    EXPECT_NEQ(Item_2.second, nullptr);
    EXPECT_EQ(Item_2.second->key, 43);
    
    EXPECT_EQ(Item_2.second->pParent, Item_1.second);
    EXPECT_EQ(Item_1.second->pRight, Item_2.second);

    auto Item_3 = Baum->InsertNode(44);
    EXPECT_EQ(Item_3.first, true);
    EXPECT_NEQ(Item_3.second, nullptr);
    EXPECT_EQ(Item_3.second->key, 44);

    EXPECT_EQ(Item_3.second->pParent, Item_2.second);
    EXPECT_EQ(Item_2.second->pRight, Item_3.second);

    auto Item_4 = Baum->InsertNode(42);
    EXPECT_EQ(Item_4.first, false);
    EXPECT_NEQ(Item_4.second, nullptr);
    EXPECT_EQ(Item_4.second->key, 42);

    auto Item_5 = Baum->InsertNode(-20);
    EXPECT_EQ(Item_5.first, true);
    EXPECT_NEQ(Item_5.second, nullptr);
    EXPECT_EQ(Item_5.second->key, -20);

    EXPECT_EQ(Item_5.second->pParent, Item_1.second);
    EXPECT_EQ(Item_1.second->pLeft, Item_5.second);


    for(int8_t i = 45; i > 0; ++i) {
        Item_1 = Baum->InsertNode(i);
        EXPECT_EQ(Item_1.first, true);
        EXPECT_NEQ(Item_1.second, nullptr);
    }
}

TEST_F(NonEmptyTree, DeleteNode_NonEmpty_001) {

    auto Item_1 = Baum->InsertNode(42);
    auto Item_2 = Baum->InsertNode(43);
    auto Item_3 = Baum->InsertNode(44);

    auto result = Baum->DeleteNode(41);
    ASSERT_EQ(result, false);

    result = Baum->DeleteNode(42);
    EXPECT_EQ(result, true);

    result = Baum->DeleteNode(42);
    EXPECT_EQ(result, false);
}

TEST_F(NonEmptyTree, FindNode_NonEmpty_001) {

    auto found = Baum->FindNode(42);
    ASSERT_EQ(found, nullptr);

    auto Item_1 = Baum->InsertNode(42);
    found = Baum->FindNode(42);
    ASSERT_EQ(found, Item_1.second);

    auto Item_2 = Baum->InsertNode(43);
    found = Baum->FindNode(43);
    EXPECT_EQ(found, Item_2.second);

    auto Item_3 = Baum->InsertNode(44);

    auto result = Baum->DeleteNode(41);
    EXPECT_EQ(result, false);

    result = Baum->DeleteNode(42);
    found = Baum->FindNode(42);
    EXPECT_EQ(result, true);
    EXPECT_EQ(found, nullptr);

    result = Baum->DeleteNode(42);
    EXPECT_EQ(result, false);
}

class TreeAxioms : public::testing::Test {

    protected:
        BinaryTree *Baum;

        void SetUp() {
            Baum = new BinaryTree();
        }

        void TearDown() {
            delete Baum;
        }
};

TEST_F(TreeAxioms, Axiom1_001) {

    auto Item = Baum->InsertNode(10);
    auto root = Baum->GetRoot();
    ASSERT_EQ(Item.first, true);
    ASSERT_EQ(Item.second, root);

    for(int i = 30; i < 75; ++i) { 
        Item = Baum->InsertNode(i);
    }

    std::vector<BinaryTree::Node_t *> field;
    Baum->GetLeafNodes(field);

    auto max_size = field.size();

    for(int full_counter = 0; full_counter < max_size; full_counter++) {
        auto color = field[full_counter]->color;
        EXPECT_EQ(color, BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2_001) {

    auto Item = Baum->InsertNode(10);

    for(int i = 30; i < 75; ++i) { 
        Item = Baum->InsertNode(i);
    }

    std::vector<BinaryTree::Node_t *> field;
    Baum->GetAllNodes(field);

    auto max_size = field.size();

    for(int full_counter = 0; full_counter < max_size; full_counter++) {
        if (field[full_counter]->color == RED) {
            auto son = field[full_counter]->pLeft->color;
            auto daughter = field[full_counter]->pRight->color;
            EXPECT_EQ(son, BLACK);
            EXPECT_EQ(daughter, BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3_001) {

    auto Item = Baum->InsertNode(10);

    for(int i = 30; i < 75; ++i) { 
        Item = Baum->InsertNode(i);
    }

    std::vector<BinaryTree::Node_t *> field;
    Baum->GetLeafNodes(field);
    auto root = Baum->GetRoot();
    auto max_size = field.size();

    auto parent = field[0]->pParent;
    int blacks_met_first_time = 0;

    while(parent != root) {
        if (parent->color == BLACK) {
            blacks_met_first_time++;
        }
        parent = parent->pParent;
    }

    for(int full_counter = 1; full_counter < max_size; full_counter++) {
        
        auto parent = field[full_counter]->pParent;
        int blacks_met = 0;

        while(parent != root) {
            if (parent->color == BLACK) {
                blacks_met++;
            }
            parent = parent->pParent;
        }
        EXPECT_EQ(blacks_met_first_time, blacks_met);

    }
}

TEST_F(TreeAxioms, Axiom3_002) {

    auto Item = Baum->InsertNode(10);

    std::vector<BinaryTree::Node_t *> field;
    Baum->GetLeafNodes(field);
    auto root = Baum->GetRoot();
    auto max_size = field.size();

    auto parent = field[0]->pParent;
    int blacks_met_first_time = 0;

    while(parent != root) {
        if (parent->color == BLACK) {
            blacks_met_first_time++;
        }
        parent = parent->pParent;
    }

    EXPECT_EQ(blacks_met_first_time, 0);
}
/*** Konec souboru black_box_tests.cpp ***/
