import React, { useState } from 'react'
import './_rulemenu.css'
import {
    TextArea,
    Button,
    Accordion,
    AccordionItem,
} from 'carbon-components-react'
import * as Feather from 'react-feather'
import { useEffect } from 'react'

class RuleObject {
    id: string
    messy_rule: string
    processed_rule: string
    type: string
    python: string

    constructor(id: string, messy_rule: string, processed_rule: string, type: string, python: string) {
        this.id = id
        this.messy_rule = messy_rule
        this.processed_rule = processed_rule
        this.type = type
        this.python = python
    }

    toString() {
        console.log("id: " + this.id + ", messy rule: " + this.messy_rule + ", processed rule: " + this.processed_rule + ", type: " + this.type + ", python: " + this.python)
    }
}
		
export const RuleMenu: React.FC = () => {
    const [rulesState, setRulesState] = useState<JSX.Element[]>([])
    useEffect(() => databaseToRules(), [])

    async function addRuleToDatabase(ruleString: string) {
         const data = new FormData();
         data.append("rule", ruleString)
         const json = await fetch("http://localhost:8000/rules/add/", 
                     {method: 'POST',
                     mode: "cors",
                     body: data
                 } )
             .then(response => {
                         return response.json();
                 })
                 .catch(error => {
                     console.error('Error: ', error);
                 });
        return json
    }

    function databaseToRules() {
        fetch("http://localhost:8000/rules/",
            {
                method: 'GET',
                mode: "cors",
            })
            .then(response => {
                return response.json();
            })
            .then(response => {
                var apiRulesObject: RuleObject[] = []
                for (var rule of response) {
                    var ruleObject: RuleObject = new RuleObject(rule.pk, rule.fields["original_input"], rule.fields["processed_text"], "", "")
                    apiRulesObject.push(ruleObject)
                }
                rulesToComponents(apiRulesObject)
            })
            .catch(error => {
                console.error('Error: ', error);
            });
    }

    async function addRule(textArea: string) {
        let textAreaObject = document.getElementById(textArea) as HTMLInputElement
        let valueOfTextArea = textAreaObject.value
        var ruleAdded = await addRuleToDatabase(valueOfTextArea)
        var badrule = document.getElementById("BadRules")
        //adding rule succeeded
        if (ruleAdded.FAIL == null) {
            badrule?.classList.remove("BadRulesShown")
            badrule?.classList.add("GoodRulesShown")
            badrule!.innerHTML = "Rule successfully added"
            databaseToRules()
            setTimeout(() => { badrule?.classList.remove("GoodRulesShown")},3000)
        }
        else {
            badrule?.classList.add("BadRulesShown")
            badrule?.classList.remove("GoodRulesShown")
            badrule!.innerHTML = ruleAdded.message
            setTimeout(() => { badrule?.classList.remove("BadRulesShown") }, 4000)
        }
    }

    function deleteFromRules(toDelete: string){

        const data = new FormData();
        data.append("pk", toDelete)
        fetch("http://localhost:8000/rules/remove/",
            {
                method: 'POST',
                mode: "cors",
                body: data
            })
            .then(response => {
                databaseToRules()
                return response.json();
            })
            .catch(error => {
                console.error('Error: ', error);
            });
    }

    function rulesToComponents(rulesObject: RuleObject[]) {
        var rulesComponents: JSX.Element[] = []
        for (let rule of rulesObject) {
            let ruleComponent: JSX.Element = <AccordionItem title={rule.messy_rule} key={rule.id} className="accordionItem">
                <TextArea 
                labelText={rule.processed_rule}
                defaultValue={rule.messy_rule}
                id={"editTextArea" + rule.id}></TextArea>
                <Button className="editButton" onClick={() => {
                    deleteFromRules(rule.id)
                    addRule("editTextArea" + rule.id)
                    }}
                >Edit</Button>
                <Button className="deleteButton" onClick={() => {deleteFromRules(rule.id)}}>
                Delete</Button>
            </AccordionItem>
            rulesComponents.push(ruleComponent)
        }
        setRulesState(rulesComponents)
    }

    return (
        <div
            id="RuleMenu"
        >
            <Button renderIcon={Feather.X} iconDescription="Close rulemenu" hasIconOnly id="closeButton"
                onClick={() => {
                    const ruleMenuObject = document.getElementById("RuleMenu");
                    ruleMenuObject?.classList.toggle("RuleMenuExtended")
                }}></Button>
        	<span 
        		id="Header"
        	>
        		Rules
            </span>
            <TextArea
                labelText=""
                placeholder="Write here your rule..."
                id="RuleTextArea"
            >
            </TextArea>
            <Button
                id="AddRuleButton"
                onClick={() => {addRule("RuleTextArea")}}
            >
                Add rule
            </Button>
            <span
                id="BadRules"
            >
                
            </span>
            <hr>
            </hr>
            <Accordion>
                {rulesState}
            </Accordion>
        </div>
    )
}

export default RuleMenu