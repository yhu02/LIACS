import React from 'react'
import configuration from "./config";

export const escapingCharacters = {
        "\n":"_NL_",
        "?":"_QM_",
        " ":"_SP_",
    };

export const escapeCharacters = function(text:string){
    for(let i in escapingCharacters){
        // @ts-ignore
        text = text.replaceAll(i,escapingCharacters[i]);
    }
    return text;
}

export const unEscapeCharacters = function(text:string){
    for(let i in escapingCharacters){
        // @ts-ignore
        text = text.replaceAll(escapingCharacters[i],i);
    }
    return text;
}



export const nl2br = function (str:string, replaceMode:boolean = true, isXhtml:boolean = true) {

    var breakTag = (isXhtml) ? '<br />' : '<br>';
    var replaceStr = (replaceMode) ? '$1'+ breakTag : '$1'+ breakTag +'$2';
    return (str + '').replace(/([^>\r\n]?)(\r\n|\n\r|\r|\n)/g, replaceStr);
}
