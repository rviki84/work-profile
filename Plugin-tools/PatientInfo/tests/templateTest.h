/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/
#ifndef TEMPLATETEST_H
#define TEMPLATETEST_H

#include "cxxtest/TestSuite.h"

/**
*\brief Tests for MyLib
*\ingroup MyLibTests
*/
class TemplateTest : public CxxTest::TestSuite 
{
public:

void TestOne()
{
   TSM_ASSERT( "TestOne run ok.", true );
}

}; // class TemplateTest

#endif TEMPLATETEST_H
