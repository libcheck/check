#ifndef CHECK_LOG_H
#define CHEKC_LOG_H

/*
  Check: a unit test framework for C
  Copyright (C) 2001, Arien Malec

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*! \page check_log Check logging
 */

/*! Set a log file to which to write during test running
  \param sr The SRunner for which to enable logging
  \param fname The file name to which to write the log
 */
void srunner_set_log (SRunner *sr, char *fname);

/*! Does the SRunner have a log file?
  \param sr The SRunner to test
  \return True if logging, False otherwise
*/
int srunner_has_log (SRunner *sr);

#endif /*CHECK_LOG_H*/
