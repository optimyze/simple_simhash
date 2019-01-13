/* Copyright 2019 optimyze.cloud AG. All Rights Reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef __TRIVIAL_HASH_H
#define __TRIVIAL_HASH_H

#include <stdint.h>

uint64_t hash_value(uint64_t value_to_hash, uint64_t hash_function_index);

#endif /* __TRIVIAL_HASH_H */

