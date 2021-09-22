function(ProvideCatch2)

    Include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        efd8cc8777406457a34e15313076ac3a77d4e54b)

    FetchContent_MakeAvailable(Catch2)

endfunction()
