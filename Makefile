DEPLOY = $(GOPATH)/bin/qtdeploy

.DEFAULT_GOAL := full

fast:
	$(DEPLOY) -fast build desktop

full: 
	$(DEPLOY) build desktop

run: 
	$(DEPLOY) run desktop

test:
	$(DEPLOY) test desktop


