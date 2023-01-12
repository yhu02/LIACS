import React,{useEffect} from 'react'
import EditorData, {IActivity} from '../../hooks/editorData'
import { 
    HTMLTable,
    Icon,
    Button,
    Alert,
    ButtonGroup,
    EditableText,
} from '@blueprintjs/core'
import _ from 'lodash'
import * as uuid from "uuid";

interface IProps{
    close:Function
}

const ActivityList: React.FC<IProps>  = ({close}) => {
    const {
        importedNodes,
        importedConnections,
        nodes,
        connections,
        writeData
    } = React.useContext(EditorData)!

    const [dialog, setDialog] = React.useState(false)
    const [activityDialog, setActivityDialog] = React.useState(false)
    const [done, setDone] = React.useState(false)
    const [error, setError] = React.useState(false)
    const { getActivities,setActivities,activities,importData,currentActivity,setCurrentActivity} = React.useContext(EditorData)!

    useEffect(() => {
        // Run! Like go get some data from an API.
        getActivities();
    }, []);

    return (
        <div style={{padding: 22, overflowY: 'scroll',textAlign:'center',fontWeight:600}}>
            <div style={{ width: '49%',display:"inline-block"}}>
                <HTMLTable
                    style={{width:'90%'}}
                    striped
                    interactive
                >
                    <thead>
                        <th>
                            <td>Activity</td>
                        </th>
                    </thead>
                    <tbody>
                    {
                        Object.keys(activities).map(
                            (activity: any, index: number) => {
                                return(
                                    <tr onClick={()=>{
                                        setCurrentActivity(activities[activity]);
                                        setActivityDialog(true)
                                    }}>
                                        <td>{activities[activity].name}</td>
                                    </tr>
                                )
                            }
                        )
                    }
                    </tbody>
                </HTMLTable>
            </div>
            <div style={{ width: '49%',display:"inline-block"}}>
                <HTMLTable
                    style={{width:'90%'}}
                    striped
                    interactive
                >
                    <thead>
                    <th>
                        <td>Applications</td>
                    </th>
                    </thead>
                    <tbody>
                    {
                        // Object.keys(activities).map(
                        //     (activity: any, index: number) => {
                        //         return(
                        //             <tr onClick={()=>{
                        //                 setDialog(true)
                        //             }}>
                        //                 <td>{activities[activity].name}</td>
                        //             </tr>
                        //         )
                        //     }
                        // )
                        <tr onClick={()=>{
                            setDialog(true)
                        }}>
                            <td>Classes</td>
                        </tr>
                    }
                    </tbody>
                </HTMLTable>
            </div>
                        <Alert
                            className='bp3-dark'
                            intent='warning'
                            confirmButtonText='Yes'
                            icon='cloud-upload'
                            cancelButtonText='No'
                            isOpen={activityDialog}
                            onClose={() => {setDialog(false);}}
                            onConfirm={() => {
                                importData(currentActivity.id,true)
                                close();
                            }}
                        >
                            You are about to load {currentActivity.name} thereby discarding all your current progress.
                        </Alert>
                        <Alert
                            className='bp3-dark'
                            intent='warning'
                            confirmButtonText='Yes'
                            icon='cloud-upload'
                            cancelButtonText='No'
                            isOpen={dialog}
                            onClose={() => {setDialog(false);}}
                            onConfirm={() => {
                                importData(0,false)
                                close();
                            }}
                        >
                            You are about to load the class diagrams thereby discarding all your current progress.
                        </Alert>
            {/*<Alert*/}
            {/*    icon="clean"*/}
            {/*    className='bp3-dark'*/}
            {/*    intent="success"*/}
            {/*    isOpen={done}*/}
            {/*    onClose={() => setDone(false)}*/}
            {/*>*/}
            {/*    Successfully pushed to backend!*/}
            {/*</Alert>*/}
            {/*<Alert*/}
            {/*    icon="clean"*/}
            {/*    className='bp3-dark'*/}
            {/*    intent="danger"*/}
            {/*    isOpen={error}*/}
            {/*    onClose={() => setError(false)}*/}
            {/*>*/}
            {/*    Couldn't push to backend!*/}
            {/*</Alert>*/}
        </div>
    )
}

export default ActivityList
