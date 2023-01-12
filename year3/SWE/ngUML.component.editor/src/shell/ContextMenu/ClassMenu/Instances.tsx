import React from 'react'
import {
    HTMLTable
} from '@blueprintjs/core'

interface IInstances {
    instances: any
}

const Instances: React.FC<IInstances> = ({instances}) => {
    return (
        <div style={{padding: 16, width: '100%'}}>
            <HTMLTable interactive striped style={{width: '100%'}}>
                <thead>
                    {Object.keys(instances).map((value: string) => { return (
                        <th>{value}</th>
                    )})}
                </thead>
                <tbody>
                    {Object.keys(instances).map((value: string) => { return (
                        <tr>
                            {instances[value].map((dataPoint: string) => {return (
                                <td>{dataPoint}</td>
                            )})}
                        </tr>
                    )})}
                </tbody>
            </HTMLTable>    
        </div>
    )
}

export default Instances
