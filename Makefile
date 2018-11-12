DEPLOY = $(GOPATH)/bin/qtdeploy

.DEFAULT_GOAL := full

fast:
	$(DEPLOY) -fast build desktop

full: 
	$(DEPLOY) build desktop

run: 
	./deploy/linux/carrierdensity

test:
	$(DEPLOY) test desktop


