import React from 'react'
import { 
    SideNav, 
    SideNavItems, 
    SideNavMenu, 
    SideNavMenuItem,
    SideNavLink
} from 'carbon-components-react'
import * as Feather from 'react-feather';
import './_nodemenu.css'
import EditorData from '../../hooks/editorData'

export const NodeMenu: React.FC = (props) => {
    const { addNode } = React.useContext(EditorData)!

    return (
        <SideNav
            isRail
            expanded={false}
            isChildOfHeader={false}
            aria-label="Side navigation"
        >
            <SideNavItems
                className='nguml-nodemenu'
            >
                <SideNavMenu 
                    renderIcon={Feather.Plus}
                    title="Class model"
                    defaultExpanded={true}
                >
                    <SideNavMenuItem
                        onClick={() => addNode('Class')}
                    >
                        Class
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode('Enum')}
                    >
                        Enum
                    </SideNavMenuItem>

                </SideNavMenu>
                <SideNavMenu
                    renderIcon={Feather.Plus}
                    title="Activity model"
                    defaultExpanded={true}
                >
                    <SideNavMenuItem
                        onClick={() => addNode('Initial')}
                    >
                        Initial
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode('Action')}
                    >
                        Action
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode('Decision')}
                    >
                        Decision
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode('Merge')}
                    >
                        Merge
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode('Fork')}
                    >
                        Fork
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode("Join")}
                    >
                        Join
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode("ActivityFinal")}
                    >
                        ActivityFinal
                    </SideNavMenuItem>
                    <SideNavMenuItem
                        onClick={() => addNode("FlowFinal")}
                    >
                        FlowFinal
                    </SideNavMenuItem>
                </SideNavMenu> 
                <SideNavLink
                    renderIcon={Feather.Plus}
                    onClick={() => {
                        const ruleMenuObject = document.getElementById("RuleMenu");
                        ruleMenuObject?.classList.toggle("RuleMenuExtended")
                        }
                    }
                >
                    Rule Menu
                </SideNavLink>
            </SideNavItems>
        </SideNav>
    )
}

export default NodeMenu