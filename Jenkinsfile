pipeline {
    agent any
    environment { 
        FLYNN_APP = 'tools'
    }
    stages {
        stage('Build') {
            steps {
                sh """set -x
                    flynn create --remote '' ${env.FLYNN_APP} || true
                    docker build -t flynn-${env.FLYNN_APP} .
                """
            }
        }
        stage('Deploy') {
            when {
                expression {
                    currentBuild.result == null || currentBuild.result == 'SUCCESS' 
                }
            }
            steps {
                sh """set -x
                    flynn -a ${env.FLYNN_APP} docker push flynn-${env.FLYNN_APP}
                    echo 'Updating SSL certs...'
                    flynn -a tools route add http tools.alecgorge.com || true
                    flynn -a ${env.FLYNN_APP} route | grep "\\:[^.]*\\.alecgorge\\.com\\" | cut -f2 | awk '{ print $3; }' | xargs -I % flynn -a ${env.FLYNN_APP} route update % -c /home/alecgorge/tls/server.crt -k /home/alecgorge/tls/server.key
                    flynn -a ${env.FLYNN_APP} scale app=1
                """
            }
        }
    }
}
