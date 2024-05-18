
// Include a header file from your module to test.
#include "ns3/shared_vars.h"

// An essential include is test.h
#include "ns3/test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// Add a doxygen group for tests.
// If you have more than one test, this should be in only one of them.
/**
 * \defgroup shared_vars-tests Tests for shared_vars
 * \ingroup shared_vars
 * \ingroup tests
 */

// This is an example TestCase.
/**
 * \ingroup shared_vars-tests
 * Test case for feature 1
 */
class Shared_varsTestCase1 : public TestCase
{
  public:
    Shared_varsTestCase1();
    virtual ~Shared_varsTestCase1();

  private:
    void DoRun() override;
};

// Add some help text to this case to describe what it is intended to test
Shared_varsTestCase1::Shared_varsTestCase1()
    : TestCase("Shared_vars test case (does nothing)")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
Shared_varsTestCase1::~Shared_varsTestCase1()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
Shared_varsTestCase1::DoRun()
{
    // A wide variety of test macros are available in src/core/test.h
    NS_TEST_ASSERT_MSG_EQ(true, true, "true doesn't equal true for some reason");
    // Use this one for floating point comparisons
    NS_TEST_ASSERT_MSG_EQ_TOL(0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined

/**
 * \ingroup shared_vars-tests
 * TestSuite for module shared_vars
 */
class Shared_varsTestSuite : public TestSuite
{
  public:
    Shared_varsTestSuite();
};

Shared_varsTestSuite::Shared_varsTestSuite()
    : TestSuite("shared_vars", UNIT)
{
    // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
    AddTestCase(new Shared_varsTestCase1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
/**
 * \ingroup shared_vars-tests
 * Static variable for test initialization
 */
static Shared_varsTestSuite sshared_varsTestSuite;
