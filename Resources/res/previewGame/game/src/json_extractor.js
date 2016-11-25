/**
 * Created by jonathan.kernick on 06/11/2015.
 */
var createFileDrop = function(domID,dropFunc,overFunc,outFunc){
    var fileDropObject = domID
    var dropFunction = function(event){
        event.stopPropagation();
        event.preventDefault();
        var files = event.target.files || event.dataTransfer.files;
        var reader = new FileReader();

        reader.onload = function(event){
            //console.log(event);
            var json = JSON.parse(event.target.result);
            dropFunc(json,files[0].name);
        };

        reader.readAsText(files[0]);
    };
    var overFunction = function(event){
        event.stopPropagation();
        event.preventDefault();
        overFunc();
        //CC.log("over");
    };
    var outFunction = function(event){
        event.stopPropagation();
        event.preventDefault();
        outFunc();
        //CC.log("out");
    };
    fileDropObject.addEventListener("dragover",overFunction,false);
    fileDropObject.addEventListener("dragleave",outFunction,false);
    fileDropObject.addEventListener("drop",dropFunction,false);

};