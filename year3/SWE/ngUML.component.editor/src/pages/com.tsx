
import React from 'react'
import styled from 'styled-components'

const ToolGrid = styled.div`
    display: flex;
    flex-direction: row;
    padding: 0.5em 0;
    gap: 0.2em;
    height: 10%;
    paddingLeft: 10;
    paddingRight: 10;
`

const componentWrapper: React.CSSProperties = {
    fontFamily: 'monospace',
    border: 'solid',
    borderColor: 'grey',
    borderWidth: 1,
    display: 'red',
    textAlign: 'center',
    backgroundColor: 'white',
    paddingTop: 8,
    paddingLeft: 8,
    paddingRight: 8,
    paddingBottom: 8,
    marginTop: 5,
}

const componentName: React.CSSProperties = {
    textAlign: 'center',
    borderWidth: 1,
    paddingTop: 8,
    paddingLeft: 8,
    paddingRight: 8,
    paddingBottom: 1,
    margin: 1
}


const Com = ({name}: any) => {
    const [arr, setArr] = React.useState<any[]>([]);

    return (
        <>
            <ToolGrid>
                {name && name?.map((e: any) => {
                    return  (
                        <div>
                        </div>
                    ) 
                })}
            </ToolGrid>
            {arr && arr?.map((e: any) => {
                    return(
                        <div>
                            {e.name}
                        </div>
                    )
            })}
        </>
    )
 }

export default Com