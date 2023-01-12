// Description: Component Modeler page

// Import the necessary libraries and files
import { useEffect, useState, useCallback } from 'react';
import Context from '../features/diagram/components/Context'
import { baseURL } from 'hooks/primitiveState'
import axios from "axios"
import { Menu } from '@nguml-editor/diagram'
import DownloadButton from '../features/diagram/components/DownloadButton';
import {
  projectAtom,
  systemAtom,
  listAtom,
  relationsAtom,
  countAtom
} from 'features/diagram/state'
import { useAtom, useAtomValue } from 'jotai'
import ReactFlow, {
  addEdge,
  applyNodeChanges,
  applyEdgeChanges,
  Node,
  Edge,
  NodeChange,
  Background,
  Controls,
  MiniMap,
  EdgeChange,
  Connection,
} from 'reactflow';
import 'reactflow/dist/style.css';
import CustomNode from '../features/diagram/components/Nodes/CustomNode';
import CircleNode from '../features/diagram/components/Nodes/CircleNode';
import SocketNodeLeft from '../features/diagram/components/Nodes/SocketNodeLeft';
import SocketNodeRight from '../features/diagram/components/Nodes/SocketNodeRight';
import SocketNodeUp from '../features/diagram/components/Nodes/SocketNodeUp';
import SocketNodeDown from '../features/diagram/components/Nodes/SocketNodeDown';
import '../features/diagram/components/componentmodeler.css';

// Define the node types
const nodeTypes: any = {
  custom: CustomNode,
  circle: CircleNode,
  socketLeft: SocketNodeLeft,
  socketRight: SocketNodeRight,
  socketUp: SocketNodeUp,
  socketDown: SocketNodeDown,
};

// Local list of nodes (for testing)
const initialNodes: Node[] = [];

// Local list of edges (for testing)
const initialEdges: Edge[] = [];

export function Flow() {

  // Set the page title
  document.title = 'ngUML Component Modeler';

  //Nodes and edges lists used by ReactFlow
  const [nodes, setNodes] = useState<Node[]>(initialNodes);
  const [edges, setEdges] = useState<Edge[]>(initialEdges);
  // Get the counter
  const counter = useAtomValue(countAtom)
  // Loading and error
  const [, setLoading] = useState<any>(true);
  const [, setError] = useState<any>(null);
  // Set the relations and components lists
  const [, setRelationships] = useAtom(relationsAtom)
  const [, setListCom] = useAtom(listAtom)
  // Get the project and system
  const project = useAtomValue(projectAtom)
  const system = useAtomValue(systemAtom)
  // Get the base URL
  const [_baseURL] = useAtom(baseURL)
  // Position of nodes
  const [posX, setPosX] = useState<any>()
  const [posY, setPosY] = useState<any>()

  // Handle the node changes (for future use)
  const onNodesChange = useCallback(
    (changes: NodeChange[]) => setNodes((nds) => applyNodeChanges(changes, nds)),
    [setNodes]
  );

  // Handle the edge changes (for future use)
  const onEdgesChange = useCallback(
    (changes: EdgeChange[]) => setEdges((eds) => applyEdgeChanges(changes, eds)),
    [setEdges]
  );

   // Called when a connection is made between two handles (for future use)
  const onConnect = useCallback(
    (connection: Connection) => setEdges((eds) => addEdge(connection, eds)),
    [setEdges]
  );

  // Endpoint for the component model
  useEffect(() => {
    const getData = async (
      baseURL: string,
      project_id: string,
      system_id: string,
    ) => { 
      try {
        const response = await axios.get(
          `${baseURL}/model/project/${project_id}/system/${system_id}/component/get_model/`
        );
        setNodes(response.data?.nodes);
        setEdges(response.data?.edges);
        setError(null);
      } catch (err: any) {
        setError(err.message);
      } finally {
        setLoading(false);
      }
    };
    getData(_baseURL, project, system);
  }, [project, system, counter, posX, posY]);

  // Endpoint for the component list
  useEffect(() => {
    const getData = async (
      baseURL: string,
      project_id: string,
      system_id: string,
    ) => { 
      try {
        const response = await axios.get(
          `${baseURL}/model/project/${project_id}/system/${system_id}/component/list_components/`
        );
        setListCom(response?.data)
      } catch (err: any) {
        setError(err.message);
      } finally {
        setLoading(false);
      }
    };
    getData(_baseURL, project, system);
  }, [project, system, counter, posX, posY]);
  


// Endpoint for the relation list
useEffect(() => {
  const getData = async (
    baseURL: string,
    project_id: string,
    system_id: string,
  ) => { 
    try {
      const response = await axios.get(
        `${baseURL}/model/project/${project_id}/system/${system_id}/component/list_relations/`
      );
      setRelationships(response?.data)
      setError(null);
    } catch (err: any) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };
  getData(_baseURL, project, system);
}, [project, system, counter, posX, posY]);

// Endpoint for the component position
const EditCompPosition = (id: any, x: any, y: any) => {
  setPosX(x)
  setPosY(y)
  fetch(`http://localhost:8000/api/model/project/${project}/system/${system}/component/edit_component/`, {

      method: 'PUT',
      headers: {
          'content-type': 'application/json;charset=UTF-8',
      },
      body: JSON.stringify({ id: id, position: { x: x, y: y } })
  })
}

// Called when the user stops dragging a node
const onNodeDragStop = useCallback(() => {
  {nodes.map((node) => (
    <div key={node.id}>
      {EditCompPosition(node.id, node.position.x, node.position.y)}
    </div>
  ))}
  });

  return (
    <div>
      <div style={{ display: 'flex', height: 900 }} >
        <ReactFlow
          // Set the nodes and edges in the component modeler
          nodes={nodes}
          edges={edges}

          onNodesChange={onNodesChange}
          onEdgesChange={onEdgesChange}
          onConnect={onConnect}
          nodeTypes={nodeTypes}
          onNodeDragStop={onNodeDragStop}
        >
          <div className="updatenode__controls">
            <Context /> 
            <br/>
            <Menu/>
            <br/>
            <DownloadButton />
          </div>
          <MiniMap
            position='bottom-left'
            nodeStrokeColor={(n: any) => {
              if (n.type === 'custom') return '#0041d0';
              if (n.type === 'circle') return '#0041d0';
              if (n.type === 'socket-down') return '#0041d0';
              if (n.type === 'socket-up') return '#0041d0';
              if (n.type === 'socket-left') return '#0041d0';
              if (n.type === 'socket-right') return '#0041d0';
            }}
            nodeColor={(n) => {
              if (n.type === 'custom') return 'rgba(240,240,240,.2)';
              return '#fff';
            }}
            zoomable pannable
          />
          <Background 
            className='bg-main'
          />
          <Controls 
            position='top-left'
            /> 
        </ReactFlow>
      
        

      </div>
    </div>
  );
}

export default Flow