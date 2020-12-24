FILE(REMOVE_RECURSE
  "CMakeFiles/my_math.dir/math.c.o"
  "../my_math.pdb"
  "../my_math.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/my_math.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
