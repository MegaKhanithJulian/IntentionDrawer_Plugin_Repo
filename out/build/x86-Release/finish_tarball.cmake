
    execute_process(
      COMMAND cmake -E rm -rf app/IntentionDrawer-0.3-msvc-wx32-10.0.19045
    )
    execute_process(
      COMMAND cmake -E rename app/files app/IntentionDrawer-0.3-msvc-wx32-10.0.19045
    )
    execute_process(
      WORKING_DIRECTORY C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release/app
      COMMAND
        cmake -E
        tar -czf ../IntentionDrawer-0.3.2-beta+2601091334.77c1be6_msvc-wx32-10.0.19045-x86.tar.gz --format=gnutar IntentionDrawer-0.3-msvc-wx32-10.0.19045
    )
    message(STATUS "Creating tarball IntentionDrawer-0.3.2-beta+2601091334.77c1be6_msvc-wx32-10.0.19045-x86.tar.gz")
  