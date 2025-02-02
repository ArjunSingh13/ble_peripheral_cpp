
Include(FetchContent)

FetchContent_Declare(
  etl
  GIT_REPOSITORY https://github.com/ETLCPP/etl
  GIT_TAG        20.39.4
  #GIT_SHALLOW   TRUE #enable this if you dont want recursive download.
)

FetchContent_MakeAvailable(etl)