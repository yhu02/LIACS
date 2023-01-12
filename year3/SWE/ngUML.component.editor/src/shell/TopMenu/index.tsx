import React from 'react'
import {
    Header,
    HeaderName,
    HeaderNavigation,
    HeaderMenuItem,
    HeaderGlobalBar,
    HeaderGlobalAction,
} from 'carbon-components-react'
import {
    Icon,
    Alert,
    Drawer,
    ButtonGroup,
    Button,
    HTMLTable,
} from '@blueprintjs/core'

import EditorData from '../../hooks/editorData'
import ChangesMenu from '../../editor/ChangesMenu'
import ActivityList from '../../editor/ActivityList'
import { useNavigate } from 'react-router-dom'
import { Home, X } from 'lucide-react'

export const TopMenu: React.FC = (props) => {
    const {
        importData,
        importDataLocal,
        references,
        autoLayout,
        getActivities,
    } = React.useContext(EditorData)!

    const [writeDialog, setWriteDialog] = React.useState(false)
    const [readDialog, setReadDialog] = React.useState(false)
    const [readCloudDialog, setReadCloudDialog] = React.useState(false)
    const [readLocalDialog, setReadLocalDialog] = React.useState(false)
    const [dataProvided, setDataProvided] = React.useState(false)

    let fileReader: any

    const handleFileRead = (e: any) => {
        const content = fileReader.result
        console.log(content)
        setDataProvided(content)
        setReadLocalDialog(true)
    }

    const handleFileChosen = (file: any) => {
        fileReader = new FileReader()
        fileReader.onloadend = handleFileRead
        fileReader.readAsText(file)
    }

    const route = useNavigate()

    return (
        <>
            <Drawer
                isOpen={writeDialog}
                onClose={() => setWriteDialog(false)}
                usePortal={true}
                portalContainer={references['root']!.current!}
                style={{
                    zIndex: 8085,
                    height: '66vh',
                    textAlign: 'center',
                }}
                className="bp3-dark"
                position="bottom"
                title="Write Changes"
            >
                <ChangesMenu />
            </Drawer>
            <Drawer
                isOpen={readDialog}
                onClose={() => setReadDialog(false)}
                usePortal={true}
                portalContainer={references['root']!.current!}
                style={{
                    zIndex: 8085,
                    height: '66vh',
                    textAlign: 'center',
                }}
                className="bp3-dark"
                position="bottom"
                title="Read / Load data"
            >
                <ActivityList
                    close={() => {
                        setReadDialog(false)
                    }}
                />

                <ButtonGroup style={{ padding: 32 }} fill>
                    <form id="LocalDataImport">
                        <HTMLTable>
                            <thead>
                                <th>
                                    <td>Local data import</td>
                                </th>
                            </thead>
                            <tbody>
                                <tr>
                                    <td>
                                        <label>
                                            Read a file from local (.json):
                                        </label>
                                    </td>
                                    <td>
                                        <input
                                            type="file"
                                            id="fileUpload"
                                            name="fileData"
                                            accept=".json"
                                            onChange={(e) => {
                                                if (e.target.files != null) {
                                                    handleFileChosen(
                                                        e.target.files[0]
                                                    )
                                                }
                                            }}
                                        />
                                    </td>
                                </tr>
                            </tbody>
                        </HTMLTable>
                    </form>
                </ButtonGroup>
                <Alert
                    isOpen={readCloudDialog}
                    onClose={() => setReadCloudDialog(false)}
                    className="bp3-dark"
                    // position='bottom'
                    // title='Load Activity'
                ></Alert>
                <Alert
                    isOpen={readLocalDialog}
                    onClose={() => setReadLocalDialog(false)}
                    className="bp3-dark"
                    icon="download"
                    intent="warning"
                    onConfirm={() => importDataLocal(dataProvided)}
                    cancelButtonText="Cancel"
                >
                    This will forcefully overwrite everything in the current
                    editor state. Are you sure?
                </Alert>
            </Drawer>
            <Header aria-label="ngUML Editor">
                <HeaderName prefix="ngUML">Editor</HeaderName>
                <HeaderNavigation aria-label="ngUML [Editor]">
                    <HeaderGlobalAction
                        aria-label="Load"
                        onClick={() => setReadDialog(true)}
                    >
                        <Icon icon="cloud-download" />
                    </HeaderGlobalAction>
                    <HeaderGlobalAction
                        aria-label="Save"
                        onClick={() => setWriteDialog(true)}
                    >
                        <Icon icon="cloud-upload" />
                    </HeaderGlobalAction>
                    <HeaderGlobalAction
                        aria-label="Layout"
                        onClick={autoLayout}
                    >
                        <Icon icon="graph" />
                    </HeaderGlobalAction>
                </HeaderNavigation>

                <HeaderGlobalBar>
                    <HeaderGlobalAction
                        aria-label="Home"
                        onClick={() => route('/')}
                    >
                        <X />
                    </HeaderGlobalAction>
                </HeaderGlobalBar>
            </Header>
        </>
    )
}

export default TopMenu
