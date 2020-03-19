pipeline {
    agent any
    stages {
        // 需要检出代码后，才可以使用代码仓库内的 Dockerfile
        stage('Checkout') {
            steps {
                checkout([
                    $class: 'GitSCM', 
                    branches: [[name: env.GIT_BUILD_REF]], 
                    userRemoteConfigs: [[url: env.GIT_REPO_URL, credentialsId: env.CREDENTIALS_ID]]])
            }
        }
        stage('Use Docker') {
            agent { 
                dockerfile {
                    filename 'Dockerfile' // 用户可以指定 Dockerfile 的名称
                    dir '/root/workspace' // 一般填写 /root/workspace, 用户也可以填写其他指定路径
                }
            }
            stages {
                stage('Test') {
                    steps {
                        sh 'node --version'
                        sh 'svn --version'
                    }
                }
            }
        }
    }
}