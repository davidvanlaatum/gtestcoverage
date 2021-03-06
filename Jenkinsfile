properties([
        disableConcurrentBuilds(),
        buildDiscarder([$class: 'EnhancedOldBuildDiscarder', artifactDaysToKeepStr: '7', artifactNumToKeepStr: '10', daysToKeepStr: '365', discardOnlyOnSuccess: false, numToKeepStr: '']),
        pipelineTriggers([upstream('docker'),cron('@weekly'), pollSCM('@hourly')])
])

node("cmake && iwyu && cppcheck && clangtidy") {
    checkout scm
    def cmake = sh returnStdout: true, script: 'which cmake3 cmake | head -n1'
    def ctest = sh returnStdout: true, script: 'which ctest3 ctest | head -n1'
    sh 'chmod a+x scripts/*'
    withEnv(["CMAKE=${cmake}", "CTEST=${ctest}", "IGNORE_EXIT_CODE=1","CC=clang", "CXX=clang++"]) {
        dir("build") {
            cache(caches: [[$class: 'ArbitraryFileCache', excludes: '', includes: '**/*.zip,**/*.tar.gz', path: 'external']], maxCacheSize: 100) {
                stage("Configure") {
                    sh 'nice ${CMAKE} -G Ninja ../ -DBUILD_TESTING=ON -DVALGRIND_XML=ON -DCPACK_BINARY_RPM=ON -DCMAKE_BUILD_TYPE=Debug -DCLANG_TIDY=ON'
                }
                stage("Build") {
                    sh 'nice ${CMAKE} --build . -- all'
                }
                stage("Check") {
                    sh 'nice ${CMAKE} --build . -- checks'
                }
                stage("Test") {
                    catchError {
                        sh 'nice ${CTEST} --verbose .'
                    }
                }
                stage("Package") {
                    sh '${CMAKE} --build . -- package'
                }
                junit '**/*-unit.xml'
                archiveArtifacts '**/*valgrind.xml,**/*-unit.xml,**/*.iwyu.log,**/*.clang_tidy.log'
                step([$class        : 'XUnitBuilder',
                      testTimeMargin: '3000',
                      thresholdMode : 1,
                      thresholds    : [
                              [$class: 'FailedThreshold', failureNewThreshold: '', failureThreshold: '', unstableNewThreshold: '', unstableThreshold: '0'],
                              [$class: 'SkippedThreshold', failureNewThreshold: '', failureThreshold: '', unstableNewThreshold: '', unstableThreshold: '0']
                      ],
                      tools         : [
                              [$class               : 'ValgrindJunitHudsonTestType',
                               deleteOutputFiles    : true,
                               failIfNotNew         : true,
                               pattern              : '**/*valgrind.xml',
                               skipNoTestFiles      : true,
                               stopProcessingIfError: true]
                      ]
                ])
                warnings parserConfigurations: [
                    [parserName: 'Clang-Tidy', pattern: '**/*.clang_tidy.log']
                ], excludePattern: '.*/include/gmock/.*,.*/include/gtest/.*', usePreviousBuildAsReference: true

                warnings consoleParsers: [
                    [parserName: 'Clang (LLVM based)']
                ], healthy: '0', unstableTotalAll: '0', excludePattern: '.*/include/gmock/.*,.*/include/gtest/.*', usePreviousBuildAsReference: true
            }
        }
    }
}
emailext body: '${JELLY_SCRIPT,template="html"}', mimeType: 'text/html', subject: 'GTestCoverage', to: 'david@vanlaatum.id.au'
