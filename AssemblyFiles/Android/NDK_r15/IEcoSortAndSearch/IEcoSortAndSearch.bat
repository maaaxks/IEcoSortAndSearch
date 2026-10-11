call ndk-build clean
call ndk-build TARGET_LINK=0 NDK_DEBUG=0 CONFIGURATION=StaticRelease
call ndk-build TARGET_LINK=0 NDK_DEBUG=1 CONFIGURATION=StaticDebug
rmdir /s /q libs
rmdir /s /q obj
call ndk-build clean
call ndk-build TARGET_LINK=1 NDK_DEBUG=0 CONFIGURATION=DynamicRelease
call ndk-build TARGET_LINK=1 NDK_DEBUG=1 CONFIGURATION=DynamicDebug
rmdir /s /q libs
rmdir /s /q obj
rem xcopy /y ..\..\..\..\BuildFiles\Android\arm64-v8a\StaticRelease\lib591FC2D6783F43F0B5B565CCCAF05939.a %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\arm64-v8a\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\arm64-v8a\DynamicRelease\lib591FC2D6783F43F0B5B565CCCAF05939.so %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\arm64-v8a\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi\StaticRelease\lib591FC2D6783F43F0B5B565CCCAF05939.a %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\armeabi\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi\DynamicRelease\lib591FC2D6783F43F0B5B565CCCAF05939.so %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\armeabi\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi-v7a\StaticRelease\lib591FC2D6783F43F0B5B565CCCAF05939.a %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\armeabi-v7a\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi-v7a\DynamicRelease\lib591FC2D6783F43F0B5B565CCCAF05939.so %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\armeabi-v7a\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips\StaticRelease\lib591FC2D6783F43F0B5B565CCCAF05939.a %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\mips\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips\DynamicRelease\lib591FC2D6783F43F0B5B565CCCAF05939.so %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\mips\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips64\StaticRelease\lib591FC2D6783F43F0B5B565CCCAF05939.a %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\mips64\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips64\DynamicRelease\lib591FC2D6783F43F0B5B565CCCAF05939.so %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\mips64\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86\StaticRelease\lib591FC2D6783F43F0B5B565CCCAF05939.a %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\x86\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86\DynamicRelease\lib591FC2D6783F43F0B5B565CCCAF05939.so %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\x86\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86_64\StaticRelease\lib591FC2D6783F43F0B5B565CCCAF05939.a %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\x86_64\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86_64\DynamicRelease\lib591FC2D6783F43F0B5B565CCCAF05939.so %ECO_FRAMEWORK%\IEcoSortAndSearch\BuildFiles\Android\x86_64\DynamicRelease\
pause
