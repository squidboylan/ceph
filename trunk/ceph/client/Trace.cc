// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */



#include "Trace.h"

#include <iostream>
#include <cassert>
#include <map>
#include <ext/rope>
using namespace __gnu_cxx;

#include "common/Mutex.h"

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



//
Trace::Trace(const char* f)
{
  fs.open(f);
  if (!fs.is_open()) {
    dout(0) << "** unable to open trace file " << f << endl;
    assert(0);
  }
  dout(2) << "opened traced file '" << f << "'" << endl;
}

Trace::~Trace()
{
  fs.close();
}


void Trace::start()
{
  //cout << "start" << endl;
  fs.seekg(0);
  
  // read first line
  getline(fs, line);
  
  _line = 1;
}

const char *Trace::get_string(char *buf, const char *prefix)
{
  if (prefix &&
      strstr(buf, "/prefix") == buf) {
    strcpy(buf, prefix);
    strcpy(buf + strlen(prefix),
	   line.c_str() + strlen("/prefix"));
  } else {
    strcpy(buf, line.c_str());
  }
  //cout << "get_string got " << buf << endl;

  // read next line (and detect eof early)
  _line++;
  getline(fs, line);
  //cout << "next line is " << line << endl;
  return buf;
}
