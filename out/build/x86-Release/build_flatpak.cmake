
    execute_process(
      COMMAND
        flatpak-builder --force-clean --keep-build-dirs
          C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release/app C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release/org.opencpn.OpenCPN.Plugin.dashboardsk.yaml
    )
    # Copy the data out of the sandbox to installation directory
    execute_process(
      WORKING_DIRECTORY C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release
      COMMAND
        flatpak-builder --run app C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release/org.opencpn.OpenCPN.Plugin.dashboardsk.yaml
           bash copy_out libIntentionDrawer.so C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release
    )
    if (NOT EXISTS app/files/lib/opencpn/libIntentionDrawer.so)
      message(FATAL_ERROR "Cannot find generated file libIntentionDrawer.so")
    endif ()
    if (RelWithDebInfo MATCHES Release|MinSizeRel)
      message(STATUS "Stripping app/files/lib/opencpn/libIntentionDrawer.so")
      execute_process(
        COMMAND strip app/files/lib/opencpn/libIntentionDrawer.so
      )
    endif ()
    execute_process(
      WORKING_DIRECTORY C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release/app
      COMMAND mv -fT files IntentionDrawer-0.3-msvc-wx32-10.0.19045
    )
    execute_process(
      WORKING_DIRECTORY  C:/Users/jbmn0/Documents/GitHub/IntentionDrawer_Plugin_Repo/intentionDrawer_pi/out/build/x86-Release/app
      COMMAND
        cmake -E
        tar -czf ../IntentionDrawer-0.3.2-beta+2601091334.77c1be6_msvc-wx32-10.0.19045-x86.tar.gz --format=gnutar IntentionDrawer-0.3-msvc-wx32-10.0.19045
    )
    message(STATUS "Building IntentionDrawer-0.3.2-beta+2601091334.77c1be6_msvc-wx32-10.0.19045-x86.tar.gz")
  