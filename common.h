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
 * @brief [brief description]
 * @details [long description]
 */
extern char * appname;

/**
 * @brief [brief description]
 * @details [long description]
 */
void print_usage(void);

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param argc [description]
 * @param argv [description]
 * 
 * @return [description]
 */
int getBufferSize(int argc, char * const * argv);

#endif