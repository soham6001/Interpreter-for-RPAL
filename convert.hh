/*
 * convert.hh
 *
 *  Created on: Apr 27, 2009
 *      Author: Saurabh
 */

#ifndef CONVERT_HH_
#define CONVERT_HH_

/*
 *      convert.hh
 *
 *      Copyright 2009 Soham Panigrahi <spanigrahi@ufl.edu>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 */

#ifndef COP5555_CONVERT
#define COP5555_CONVERT
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <stdexcept>

 inline std::string toString(int x)
 {
   std::ostringstream o;
   if (!(o << x))
     return " ";
   return o.str();
 }

  inline int toInt(const std::string& s)
 {
   std::istringstream i(s);
   int x;
   if (!(i >> x))
     return 0;
   return x;
 }
#endif


#endif /* CONVERT_HH_ */
