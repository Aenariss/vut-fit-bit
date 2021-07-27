//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Vojtech Fiala <xfiala61@stud.fit.vutbr.cz>
// $Date:       $2021-03-03
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Vojtech Fiala
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include <exception> 
#include "white_box_code.h"


class BasicMatrix : public::testing::Test {

    protected:
        Matrix *Neo;

        void SetUp() {
            Neo = new Matrix();
        }

        void TearDown() {
            delete Neo;
        }
};

TEST_F(BasicMatrix, Create_Invalid_size) {
    Matrix *Trinity;

    try {
        Trinity = new Matrix(0,1);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Minimalni velikost matice je 1x1");
    }

}

TEST_F(BasicMatrix, BasicGet) {

    auto value = Neo->get(0,0);
    ASSERT_EQ(value, 0);
}

TEST_F(BasicMatrix, BasicGet_Invalid) {

    try {
        auto value = Neo->get(1,0);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Pristup k indexu mimo matici");
    }
}

TEST_F(BasicMatrix, BasicSet) {

    auto result = Neo->set(0,0,10);
    ASSERT_EQ(result, true);

    auto value = Neo->get(0,0);
    ASSERT_EQ(value, 10);
}

TEST_F(BasicMatrix, BasicSet_Invalid) {

    auto result = Neo->set(1,0,10);
    ASSERT_EQ(result, false);

}

TEST_F(BasicMatrix, BasicSet_Vector) {

    std::vector<std::vector< double > > values = {{10}};
    auto result = Neo->set(values);
    ASSERT_EQ(result, true);

    auto value = Neo->get(0,0);
    ASSERT_EQ(value, 10);
}

TEST_F(BasicMatrix, BasicSet_Vector_Invalid) {

    std::vector<std::vector< double > > values = {{10},{20}};
    auto result = Neo->set(values);
    ASSERT_EQ(result, false);

}

TEST_F(BasicMatrix, Invalid_Construct) { 
    Matrix *Morpheus;

    try {
        Morpheus = new Matrix(0,42);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Minimalni velikost matice je 1x1");
    }
}

class AdvancedMatrix : public::testing::Test {

    protected:
        Matrix *Neo;

        void SetUp() {
            Neo = new Matrix(3,3);
        }

        void TearDown() {
            delete Neo;
        }
};

TEST_F(AdvancedMatrix, Get) {

    for(int i = 0; i < 3; i++) {
        for(int k = 0; k < 3; k++) {
            auto value = Neo->get(i,k);
            ASSERT_EQ(value, 0);
        }
    }
}

TEST_F(AdvancedMatrix, Set) {
    
    std::vector<std::vector< double > > values = {{10,10,10},{10,20,10},{10,10,10}};
    auto result = Neo->set(values);
    ASSERT_EQ(result, true);

    for(int i = 0; i < 3; i++) {
        for(int k = 0; k < 3; k++) {
            if (i == 1 && k == 1) {
                auto value = Neo->get(i,k);
                EXPECT_EQ(value, 20);
            }
            else {
                auto value = Neo->get(i,k);
                EXPECT_EQ(value, 10);
            }
        }
    }
}

TEST_F(AdvancedMatrix, Set_Invalid) {
    
    std::vector<std::vector< double > > values = {{10,10},{10,20,10},{10,10,10}};
    std::vector<std::vector< double > > values_2 = {{10,10,10},{10,20,10}};
    auto result = Neo->set(values);
    auto result_2 = Neo->set(values_2);
    ASSERT_EQ(result, false);
    ASSERT_EQ(result_2, false);
}

class AdvancedMatrixOperations : public::testing::Test {

    protected:
        Matrix *Neo;
        std::vector<std::vector< double > > values = {{10,10,10},{10,20,10},{10,10,10}};

        void SetUp() {
            Neo = new Matrix(3,3);
            Neo->set(values);
        
        }

        void TearDown() {
            delete Neo;
        }
};

TEST_F(AdvancedMatrixOperations, Equal) {
    
    Matrix *Morpheus;
    Morpheus = new Matrix(3,3);
    Morpheus->set(values);

    auto result = Neo->operator==(*Morpheus);
    ASSERT_EQ(result, true);

    delete Morpheus;

}

TEST_F(AdvancedMatrixOperations, Equal_Different) {
    
    Matrix *Morpheus;
    std::vector<std::vector< double > > check_values = {{10,10,10},{10,30,10},{10,10,10}};
    Morpheus = new Matrix(3,3);
    Morpheus->set(check_values);

    auto result = Neo->operator==(*Morpheus);
    ASSERT_EQ(result, false);

    delete Morpheus;

}

TEST_F(AdvancedMatrixOperations, Equal_Different_Size) {
    
    Matrix *Morpheus;
    std::vector<std::vector< double > > check_values = {{10,30,10},{10,10,10}};
    Morpheus = new Matrix(2,3);
    Morpheus->set(check_values);

    try {
        auto result = Neo->operator==(*Morpheus);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice musi mit stejnou velikost.");
    }

    delete Morpheus;

}

TEST_F(AdvancedMatrixOperations, Add_together) {
    
    Matrix *Morpheus;
    std::vector<std::vector< double > > check_values = {{10,10,10},{10,0,10},{10,10,10}};
    Morpheus = new Matrix(3,3);
    Morpheus->set(check_values);

    auto Trinity = Neo->operator+(*Morpheus);
    for(int i = 0; i < 3; i++) {
        for(int k = 0; k < 3; k++) {
            ASSERT_EQ(Trinity.get(i, k), 20);
        }
    }

    delete Morpheus;

}

TEST_F(AdvancedMatrixOperations, Add_together_invalid_size) {
    
    Matrix *Morpheus;
    std::vector<std::vector< double > > check_values = {{10,10},{10,0},{10,10}};
    Morpheus = new Matrix(2,3);
    Morpheus->set(check_values);

    try {
        auto Trinity = Neo->operator+(*Morpheus);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice musi mit stejnou velikost.");
    }

    delete Morpheus;

}

TEST_F(AdvancedMatrixOperations, Multiply_together) {
    
    Matrix *Morpheus;
    std::vector<std::vector< double > > check_values = {{10,10,10},{10,0,10},{10,10,10}};
    Morpheus = new Matrix(3,3);
    Morpheus->set(check_values);

    auto Trinity = Neo->operator*(*Morpheus);
    for(int i = 0; i < 3; i++) {
        for(int k = 0; k < 3; k++) {
            if (k == 1) {
                ASSERT_EQ(Trinity.get(i, k), 200);
            }
            else if ((i == 0 || i == 2) && (k == 0 || k == 2)) {
                ASSERT_EQ(Trinity.get(i, k), 300);
            }
            else {
                ASSERT_EQ(Trinity.get(i, k), 400);
            }
        }
    }

    delete Morpheus;

}

TEST_F(AdvancedMatrixOperations, Multiply_together_invalid) {
    
    Matrix *Morpheus;
    std::vector<std::vector< double > > check_values = {{10,10},{10,0},{10,10}};
    Morpheus = new Matrix(2,3);
    Morpheus->set(check_values);

    try {
        auto Trinity = Neo->operator*(*Morpheus);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Prvni matice musi stejny pocet sloupcu jako druha radku.");
    }

    delete Morpheus;

}

TEST_F(AdvancedMatrixOperations, Multiply_together_scalar) {
    
    double scalar = 5;
    auto Trinity = Neo->operator*(scalar);
    for(int i = 0; i < 3; i++) {
        for(int k = 0; k < 3; k++) {
            if (k == 1 && i == 1) {
                ASSERT_EQ(Trinity.get(i, k), 100);
            }
            else {
                ASSERT_EQ(Trinity.get(i, k), 50);
            }
        }
    }
}

class SolveEquations : public::testing::Test {

    protected:
        Matrix *Neo;
        std::vector<std::vector< double > > values = {{4,2},{5,-3}};

        void SetUp() {
            Neo = new Matrix(2,2);
            Neo->set(values);
        
        }

        void TearDown() {
            delete Neo;
        }
};

TEST_F(SolveEquations, SolveEquation) {
    
    std::vector< double > results = {6,13};
    auto Trinity = Neo->solveEquation(results);
    
    ASSERT_EQ(Trinity[0], 2);
    ASSERT_EQ(Trinity[1], -1);
}

TEST_F(SolveEquations, SolveEquation_invalid_size) {
    
    std::vector< double > results = {6,13,15};
    try {
        auto Trinity = Neo->solveEquation(results);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Pocet prvku prave strany rovnice musi odpovidat poctu radku matice.");

    }
}

TEST_F(SolveEquations, SolveEquation_invalid_size_2) {
    
    Matrix *Trinity;
    Trinity = new Matrix(2,3);
    std::vector< double > results = {6,13,15};
    std::vector<std::vector< double > > values = {{4,2},{5,-3},{8,1}};
    Trinity->set(values);

    try {
        auto Morpheus = Trinity->solveEquation(results);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice musi byt ctvercova.");

    }

    delete Trinity;
}

TEST_F(SolveEquations, SolveEquation_singular) {
    
    Matrix *Trinity;
    Trinity = new Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,1},{1,1}};
    Trinity->set(values);
    std::vector< double > results = {1,3};

    try {
        auto Morpheus = Trinity->solveEquation(results);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice je singularni.");

    }

    delete Trinity;
}

TEST_F(SolveEquations, SolveEquation_singular_2) {
    
    std::vector< double > results = {0,0};
    Matrix *Trinity;
    Trinity = new Matrix(2,2);
    std::vector<std::vector< double > > values = {{5,5},{5,5}};
    Trinity->set(values);

    try {
        auto Morpheus = Trinity->solveEquation(results);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice je singularni.");

    }

    delete Trinity;
}

TEST_F(SolveEquations, SolveEquation_1x1) {
    
    std::vector< double > results = {0};
    Matrix *Trinity;
    Trinity = new Matrix();
    std::vector<std::vector< double > > values = {{0}};
    Trinity->set(values);

    try {
        auto Morpheus = Trinity->solveEquation(results);
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice je singularni.");

    }

    delete Trinity;
}

TEST_F(SolveEquations, SolveEquation_big) {
    
    std::vector< double > results = {3,2,3,5};
    Matrix *Trinity;
    Trinity = new Matrix(4,4);
    std::vector<std::vector< double > > values = {{2,1,1,2},{3,1,1,3},{1,2,3,1},{1,3,1,2}};
    Trinity->set(values);

    auto Morpheus = Trinity->solveEquation(results);

    ASSERT_EQ(Morpheus[0], 20);
    ASSERT_EQ(Morpheus[1], 11);
    ASSERT_EQ(Morpheus[2], -6);
    ASSERT_EQ(Morpheus[3], -21);

    delete Trinity;
}

TEST_F(SolveEquations, SolveEquation_Big_2) {
    
    std::vector< double > results = {1};
    Matrix *Trinity;
    Trinity = new Matrix();
    std::vector<std::vector< double > > values = {{1}};
    Trinity->set(values);

    auto Morpheus = Trinity->solveEquation(results);
    ASSERT_EQ(Morpheus[0], 1);
    

    delete Trinity;
}

class Transpose : public::testing::Test {

    protected:
        Matrix *Neo;
        std::vector<std::vector< double > > values = {{4,2},{5,-3}};

        void SetUp() {
            Neo = new Matrix(2,2);
            Neo->set(values);
        
        }

        void TearDown() {
            delete Neo;
        }
};

TEST_F(Transpose, Transpose_1) {
    
    Matrix *Trinity;
    Trinity = new Matrix(2,2);
    std::vector<std::vector< double > > values = {{4,5},{2,-3}};
    Trinity->set(values);

    auto Morpheus = Neo->transpose();
    auto result = Morpheus.operator==(*Trinity);
    ASSERT_EQ(result, true);
}

class Inverse : public::testing::Test {

    protected:
        Matrix *Neo;
        Matrix *Trinity;
        std::vector<std::vector< double > > values = {{4,6},{6,5}};
        std::vector<std::vector< double > > values_2 = {{1,2,2},{2,3,2},{2,2,2}};

        void SetUp() {
            Neo = new Matrix(2,2);
            Neo->set(values);

            Trinity = new Matrix(3,3);
            Trinity->set(values_2);
        
        }

        void TearDown() {
            delete Neo;
            delete Trinity;
        }
};

TEST_F(Inverse, Inverse_1) {
    
    Matrix *Trinity;
    Trinity = new Matrix(2,2);
    std::vector<std::vector< double > > values_test = {{-0.3125, 0.375}, {0.375,-0.25}};
    Trinity->set(values_test);

    auto Morpheus = Neo->inverse();
    auto result = Morpheus.operator==(*Trinity);
    ASSERT_EQ(result, true);
}

TEST_F(Inverse, Inverse_2) {
    
    Matrix *Reloaded;
    Reloaded = new Matrix(3,3);
    std::vector<std::vector< double > > values_test = {{-1,0,1},{0,1,-1},{1,-1,0.5}};
    Reloaded->set(values_test);

    auto Morpheus = Trinity->inverse();
    auto result = Morpheus.operator==(*Reloaded);
    ASSERT_EQ(result, true);
}

TEST_F(Inverse, Inverse_invalid_size) {
    
    Matrix *Reloaded;
    Reloaded = new Matrix(2,4);
    std::vector<std::vector< double > > values_test = {{-1,0},{0,1},{1,-1},{0,4}};
    Reloaded->set(values_test);

    try {
        auto Morpheus = Reloaded->inverse();
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice musi byt velikosti 2x2 nebo 3x3.");
    }
}

TEST_F(Inverse, Inverse_invalid_singular) {
    
    Matrix *Reloaded;
    Reloaded = new Matrix(2,2);
    std::vector<std::vector< double > > values_test = {{2,2},{4,4}};
    Reloaded->set(values_test);

    try {
        auto Morpheus = Reloaded->inverse();
    }
    catch (std::exception &except) {
        ASSERT_STREQ(except.what(), "Matice je singularni.");
    }
}

/*** Konec souboru white_box_tests.cpp ***/
