import React from 'react'

const environment : string = 'dev'; // ENUM: dev or live

let configuration = {
    url:'',
    environment:environment,
};
// @ts-ignore
switch (environment){
    case "dev":
        configuration['url'] = 'http://localhost:8000/';
        break;
    case "live":
    default:
        configuration['url'] = 'https://model.nguml.com/';
        break;
}

export default configuration;
