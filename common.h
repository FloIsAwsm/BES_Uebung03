/**
 * @file common.h
 * 
 * Beispiel 2
 * 
 * @author Florian Froestl <florian.froestl@technikum-wien.at>
 * @author David Boisits <david.boisits@technikum-wien.at>
 * 
 * @date 2016/06/03
 * 
 * @version 100
 * 
 */
#ifndef COMMON_H
#define COMMON_H

/**
 * global variable that stores the application name
 */
extern char * appname;

/**
 * @brief prints the correct usage of the application
 */
void print_usage(void);

/**
 * @brief parses the command line arguments
 * @details sets the application name and parses the command line arguments
 * for the buffer size with the help of getopt().
 * 
 * @param argc number of command line arguments
 * @param argv array of strings containing the command line arguments
 * 
 * @return the buffer size specified in the command line argument
 */
int getBufferSize(int argc, char * const * argv);

#endif