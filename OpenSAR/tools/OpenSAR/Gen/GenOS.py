"""/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
"""
import sys,os
from GCF import *

__all__ = ['GenOS']

__dir = '.'

def GenOS(root,dir):
    global __dir
    GLInit(root)
    __dir = '%s'%(dir)
    GenH()
    GenC()
    print '>>> Gen OS DONE <<<'

def GenH():
    global __dir
    fp = open('%s/Os_Cfg.h'%(__dir),'w')
    fp.write(GHeader('Os'))
    fp.write("""
#if !(((OS_SW_MAJOR_VERSION == 2) && (OS_SW_MINOR_VERSION == 0)) )
#error Os: Configuration file expected BSW module version to be 2.0.*
#endif

#ifndef OS_CFG_H_
#define OS_CFG_H_

// Application Id's
#define APPLICATION_ID_OsDefaultApplication  0  // TODO: 

// Alarm Id's
""")
    alarmid = 0
    for Alarm in GLGet('AlarmList'):
        fp.write('#define ALARM_ID_%s %s\n'%(GAGet(Alarm,'Name'),alarmid))
        alarmid += 1
    fp.write('\n// Counter Id\'s\n')
    counterid = 0
    for Counter in GLGet('CounterList'):
        fp.write('#define COUNTER_ID_%s %s\n'%(GAGet(Counter,'Name'),counterid))
        counterid += 1
    fp.write("""
// System counter TODO
#define OSMAXALLOWEDVALUE        UINT_MAX// NOT CONFIGURABLE IN TOOLS
#define OSTICKSPERBASE            1       // NOT CONFIGURABLE IN TOOLS
#define OSMINCYCLE                1        // NOT CONFIGURABLE IN TOOLS
#define OSTICKDURATION            1000000UL    // Time between ticks in nano seconds

// Counter macros
""")
    counterid = 0
    for Counter in GLGet('CounterList'):
        if(counterid == 0):
            max = 'OSMAXALLOWEDVALUE //TODO: I set the first counter configured by easySAR as system counter, sorry' 
        else:
            max = GAGet(Counter,'maxallowedvalue')
        fp.write('#define OSMAXALLOWEDVALUE__%s %s\n'%(GAGet(Counter,'Name'),max))
        fp.write('#define OSTICKSPERBASE_%s 1 // NOT CONFIGURABLE IN TOOLS,sorry\n'%(GAGet(Counter,'Name')))
        fp.write('#define OSMINCYCLE_%s %s\n'%(GAGet(Counter,'Name'),GAGet(Counter,'MinCycle')))
        if(counterid == 0):
            fp.write("""
#define OS_TICKS2SEC_%s(_ticks)       ( (OSTICKDURATION * _ticks)/1000000000UL )
#define OS_TICKS2MS_%s(_ticks)        ( (OSTICKDURATION * _ticks)/1000000UL )
#define OS_TICKS2US_%s(_ticks)        ( (OSTICKDURATION * _ticks)/1000UL )
#define OS_TICKS2NS_%s(_ticks)        (OSTICKDURATION * _ticks)
            """%(GAGet(Counter,'Name'),GAGet(Counter,'Name'),GAGet(Counter,'Name'),GAGet(Counter,'Name')) )
        counterid += 1
    fp.write('\n// Event masks\n')
    for Task in GLGet('TaskList'):
        for Event in GLGet(Task,'EventList'):
            fp.write('#define EVENT_MASK_%s %s // of %s\n'%(GAGet(Event,'Name'),GAGet(Event,'Mask'),GAGet(Task,'Name')))
    fp.write("""
// Isr Id's

// Resource Id's

// Linked resource id's

// Resource masks

// Task Id's
""")  
    fp.write('#define TASK_ID_OsIdle    0 // TODO, generate it by default\n')
    taskid = 1
    for Task in GLGet('TaskList'):
        fp.write('#define TASK_ID_%s %s\n'%(GAGet(Task,'Name'),taskid))
        taskid += 1
    fp.write('\n// Task entry points\n')
    fp.write('extern void OsIdle( void );\n')
    for Task in GLGet('TaskList'):
        fp.write('extern void %s( void );\n'%(GAGet(Task,'Name')))
    fp.write('\n// Schedule table id\'s\n')
    fp.write("""
// Stack size
#define OS_INTERRUPT_STACK_SIZE    2048    // TODO
#define OS_OSIDLE_STACK_SIZE 512            // TODO

""")
    fp.write('#define OS_ALARM_CNT            %s\n'%(len(GLGet('AlarmList'))))
    fp.write('#define OS_TASK_CNT             %s\n'%(len(GLGet('TaskList'))+1))
    fp.write('#define OS_SCHTBL_CNT           %s\n'%(len(GLGet('ScheduleTableList'))))
    fp.write('#define OS_COUNTER_CNT          %s\n'%(len(GLGet('CounterList'))))
    EventCount = 0
    for Task in GLGet('TaskList'):
        for Event in GLGet(Task,'EventList'):
            EventCount += 1
    fp.write('#define OS_EVENTS_CNT           %s\n'%(EventCount))
    fp.write("""
// TODO: 
//#define OS_ISRS_CNT                 0
#define OS_RESOURCE_CNT               0
#define OS_LINKED_RESOURCE_CNT        0
#define OS_APPLICATION_CNT            1    // TODO
#define OS_SERVICE_CNT                0  /* ARCTICSTUDIO_GENERATOR_TODO */
#define CFG_OS_DEBUG                  STD_ON

#define OS_SC1                        STD_ON     /* NOT CONFIGURABLE IN TOOLS */
#define OS_USE_APPLICATIONS           STD_ON
#define OS_USE_MEMORY_PROT            STD_OFF    /* NOT CONFIGURABLE IN TOOLS */
#define OS_USE_TASK_TIMING_PROT       STD_OFF    /* NOT CONFIGURABLE IN TOOLS */
#define OS_USE_ISR_TIMING_PROT        STD_OFF    /* NOT CONFIGURABLE IN TOOLS */
//#define OS_SC3                      STD_ON     /* NOT CONFIGURABLE IN TOOLS */  
#define OS_STACK_MONITORING           STD_ON
#define OS_STATUS_EXTENDED            STD_ON
#define OS_USE_GET_SERVICE_ID         STD_ON     /* NOT CONFIGURABLE IN TOOLS */
#define OS_USE_PARAMETER_ACCESS       STD_ON     /* NOT CONFIGURABLE IN TOOLS */
#define OS_RES_SCHEDULER              STD_ON     /* NOT CONFIGURABLE IN TOOLS */

#define OS_ISR_CNT          0
#define OS_ISR2_CNT         0
#define OS_ISR1_CNT         0

#define OS_ISR_MAX_CNT      10

#define OS_NUM_CORES        1
""")
    fp.write('#endif /*OS_CFG_H_*/\n\n')
    fp.close()
    
def GenC():
    global __dir,__root
    global __dir,__root
    fp = open('%s/Os_Cfg.c'%(__dir),'w')
    fp.write(GHeader('Os'))
    fp.write("""
#include "kernel.h"


// ###############################    EXTERNAL REFERENCES    #############################

/* Application externals */

/* Interrupt externals */


// Set the os tick frequency
OsTickType OsTickFreq = 1000;


// ###############################    DEBUG OUTPUT     #############################
uint32 os_dbg_mask = 0;  //D_TASK|D_ALARM|D_EVENT;

// ###############################    APPLICATIONS     #############################
GEN_APPLICATION_HEAD = {
    GEN_APPLICATION(
                /* id           */ APPLICATION_ID_OsDefaultApplication,
                /* name         */ "OsDefaultApplication",
                /* trusted      */ true,    /* NOT CONFIGURABLE IN TOOLS */
                /* core         */ 0, /* Default value, multicore not enabled.*/
                /* StartupHook  */ NULL,
                /* ShutdownHook */ NULL,
                /* ErrorHook    */ NULL,
                /* rstrtTaskId  */ 0    /* NOT CONFIGURABLE IN TOOLS */
                ),                    
};  
    """)
    cstr = '\n// #################################    COUNTERS     ###############################\nGEN_COUNTER_HEAD = {\n'
    for Counter in GLGet('CounterList'):
        cstr +=     \
        """\tGEN_COUNTER(    COUNTER_ID_%s,
                    "%s",
                    COUNTER_TYPE_SOFT,     // Default
                    COUNTER_UNIT_TICKS,    // Default
                    %s,                // maxallowedvalue
                    %s,                // ticksperbase
                    %s,                // mincycle
                    0,            // TODO:Gpt Channel
                    APPLICATION_ID_OsDefaultApplication,    /* Application owner */
                    1                                       /* Accessing application mask */
                ),\n"""%(GAGet(Counter,'Name'), \
             GAGet(Counter,'Name'), \
             GAGet(Counter,'MaxAllowed'),  \
             GAGet(Counter,'TicksPerBase'), \
             GAGet(Counter,'MinCycle') )
    cstr += '};\n\n'
    fp.write(cstr)
    for Counter in GLGet('CounterList'):
        fp.write('\tCounterType Os_Arc_OsTickCounter = COUNTER_ID_%s;\n'%(GAGet(Counter,'Name')))
        break;
    cstr = '\n// ##################################    ALARMS     ################################\n'
    for Alarm in GLGet('AlarmList'):
        if(GAGet(Alarm,'Autostart') == 'False'):
            continue
        cstr += 'GEN_ALARM_AUTOSTART(ALARM_ID_%s, ALARM_AUTOSTART_%s, %s, %s, %s );\n'%(
            GAGet(Alarm,'Name'),GAGet(Alarm,'Autostart').upper(),GAGet(Alarm,'StartTime'),GAGet(Alarm,'Period'),'OSDEFAULTAPPMODE')
    fp.write(cstr)
    cstr = 'GEN_ALARM_HEAD = {\n'
    for Alarm in GLGet('AlarmList'):
        if(GAGet(Alarm,'Autostart') == 'False'):
            cauto = 'NULL'
        else:
            cauto = 'GEN_ALARM_AUTOSTART_NAME(ALARM_ID_%s)'%(GAGet(Alarm,'Name'))
        if(GAGet(Alarm,'Action') == 'SetEvent'):
            ctask =  'TASK_ID_%s'%(GAGet(Alarm,'Task'))
            cevent = 'EVENT_MASK_%s'%(GAGet(Alarm,'Event'))
            counter = '0'
        elif(GAGet(Alarm,'Action') == 'ActivateTask'):
            ctask =  'TASK_ID_%s'%(GAGet(Alarm,'Task'))
            cevent = '0'
            counter = '0'
        elif(GAGet(Alarm,'Action') == 'AlarmCallBack'):
            ctask =  '0'
            cevent = '0'
            counter = '0'
        else:
            ctask =  '0'
            cevent = '0'
            counter = 'COUNTER_ID_%s'%(GAGet(Alarm,'Counter'))
        cstr += """\tGEN_ALARM(  ALARM_ID_%s,
                "%s",
                COUNTER_ID_%s,
                %s,
                ALARM_ACTION_%s,
                %s,
                %s,
                %s,
                APPLICATION_ID_OsDefaultApplication,    /* Application owner */
                1    /* Accessing application mask */
            ),\n """%(GAGet(Alarm,'Name'),GAGet(Alarm,'Name'),GAGet(Alarm,'Counter'),
                      cauto,
                      GAGet(Alarm,'Action').upper(),
                      ctask,cevent,counter)
    cstr += '};\n\n'
    fp.write(cstr)
    fp.write('// ################################    RESOURCES     ###############################\n\n')
    fp.write('// ##############################    STACKS (TASKS)     ############################\n')
    fp.write('DECLARE_STACK(OsIdle,OS_OSIDLE_STACK_SIZE);\n')
    for Task in GLGet('TaskList'):
        fp.write('DECLARE_STACK(%s,%s);\n'%(GAGet(Task,'Name'),GAGet(Task,'StackSize')))
    cstr = """// ##################################    TASKS     #################################
GEN_TASK_HEAD = {
    GEN_BTASK(  /*                     */OsIdle,
                /* name                */"OsIdle",
                /* priority            */0,
                /* schedule            */FULL,
                /* autostart           */TRUE,
                /* resource_int_p   */NULL,
                /* resource mask    */0,
                /* activation lim.     */1,
                /* App owner        */0,
                /* Accessing apps   */1 
    ),\n"""
    for Task in GLGet('TaskList'):
        if(len(GLGet(Task,'EventList')) > 0):
            cmask = 0
            for Event in GLGet(Task,'EventList'):
                cmask = cmask|Integer(GAGet(Event,'Mask'))
            cstr += """\tGEN_ETASK(
        /*                     */%s,
        /* name                */"%s",
        /* priority            */%s,
        /* schedule            */%s,
        /* autostart           */%s,
        /* resource_int_p   */NULL, // TODO
        /* resource mask    */0,
        /* event mask        */%s,
        /* App owner        */APPLICATION_ID_OsDefaultApplication,
        /* Accessing apps   */1
    ),\n"""%(GAGet(Task,'Name'),GAGet(Task,'Name'),GAGet(Task,'Priority'),GAGet(Task,'Schedule'),
             GAGet(Task,'Autostart'),hex(cmask))
        else:
            cstr += """\tGEN_BTASK(
        /*                     */%s,
        /* name                */"%s",
        /* priority            */%s,
        /* schedule            */%s,
        /* autostart           */%s,
        /* resource_int_p   */NULL,    // TODO
        /* resource mask    */0,
        /* activation lim.     */%s,
        /* App owner        */APPLICATION_ID_OsDefaultApplication,
        /* Accessing apps   */1
    ),\n"""%(GAGet(Task,'Name'),GAGet(Task,'Name'),GAGet(Task,'Priority'),GAGet(Task,'Schedule'),
             GAGet(Task,'Autostart'),GAGet(Task,'Activation'))
    cstr += '};\n\n'
    fp.write(cstr)
    fp.write("""
// ##################################    HOOKS     #################################
GEN_HOOKS( 
    StartupHook, 
    NULL, 
    ShutdownHook, 
    ErrorHook,
    PreTaskHook, 
    PostTaskHook 
);

// ##################################    ISRS     ##################################

GEN_ISR_MAP = {
    0
};

// ############################    SCHEDULE TABLES     #############################
    
    """)
    fp.close()