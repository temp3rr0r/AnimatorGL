// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/concurrent_vector.h>
#include <tbb/cache_aligned_allocator.h>
#include <tbb/concurrent_vector.h>
#include <tbb/tick_count.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <tiffio.h>
#include "Particle.h"
#include "ParticleHandler.h"
#include "Settings.h"
#include "QuadParticleTree.h"
#include "TreeParticle.h"