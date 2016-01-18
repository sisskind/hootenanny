# Installing Hootennany on AWS Instance (Ubuntu)

Log into AWS account and launch an AWS Ubuntu 14.04 instance.

MINIMUM AWS INSTANCE SIZE: 
		m4.xlarge
RECOMMENDED STORAGE: 50GB
RECOMMENDED: Assign Public IP to instance

CREATE AWS SECURITY GROUPS to allow these ports for Hootenanny
		Custom TCP Rule - Port Range: 8096 = P2P Server Port
		Custom TCP Rule - Port Range: 8080 = Tomcat Web Server Port
		Custom TCP Rule - Port Range: 8094 = Translation Server Port
		TCP Rule - Port Range: 22 = SSH into the the AWS Instance
		
ITEMS Needed: AWS PEM file to ssh into the AWS instance to install Hootenanny. User can create this before launching the AWS Instance.

# Setting up Hootennany

Once Instance is in "running" state, ssh into instance from command line (putty)

Start by checking out the hoot code, then changing to the branch by typing:

    01) sudo apt-get -y install git
    02) git clone https://github.com/ngageoint/hootenanny.git hoot​
    03) sudo mv hoot /usr/local/
    04) sudo chown ubuntu:ubuntu /usr/local/hoot
    05) cd /usr/local/hoot
    06) git checkout <BRANCH_NAME>  #i.e. Replace <BRANCH_NAME> with: "develop", "release", "master" -- additional branches can be found: https://github.com/ngageoint/hootenanny/branches
    07) git status 		
    08) git submodule init
    09) git submodule update
    10) chmod +x ./AWSProvision-Ubuntu1404.sh
        If this fails, run:
            sudo apt-get -y install wget
            wget https://raw.githubusercontent.com/ngageoint/hootenanny/AWS_Provision/AWSProvision-Ubuntu1404.sh
            
        Then re-run step 10
    11) sudo ./AWSProvision-Ubuntu1404.sh
    12) source /usr/local/hoot/SetupEnv.sh
	
To access the web pages you access the site in your [local Chrome browser](http://localhost:8080/hootenanny-id).

TO UPDATE HOOT CODE:
If you want the latest hoot code, connect to the AWS Instance via ssh to build and redeploy to Tomcat:

    01) ssh to AWS Instance
    02) cd /usr/local/hoot
    03) source SetupEnv.sh
	04) git status # Check to see which branch is selected
		05) OPTIONAL: Change branch to build different hoot code: git checkout <BRANCH_NAME>
	06) git pull
    07) scripts/ezClean.sh
	08) cp ${HOOT_HOME}/conf/DatabaseConfig.sh.orig ${HOOT_HOME}/conf/DatabaseConfig.sh
    09) scripts/ezBuildAll.sh
    10) sudo -u tomcat6 scripts/vagrantDeployTomcat.sh
	11) sudo service tomcat6 restart
	12) source ${HOOT_HOME}/SetupEnv.sh
