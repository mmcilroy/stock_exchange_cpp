echo "#pragma once" > events.hpp
echo "" >> events.hpp
echo "#include <cstdint>" >> events.hpp
echo "" >> events.hpp
lua gen_events.lua >> events.hpp

echo "#pragma once" > pack.hpp
echo "" >> pack.hpp
echo "#include \"events.hpp\"" >> pack.hpp
echo "" >> pack.hpp
lua gen_pack.lua pack.hpp >> pack.hpp

mv *.hpp ../include

