import { Project, System } from '@nguml-editor/browser/types'
import {
    DataTable,
    DataTableHeader,
    Table,
    TableBody,
    TableCell,
    TableContainer,
    TableHead,
    TableHeader,
    TableRow,
} from 'carbon-components-react'
import React from 'react'
import { useLocation, useNavigate } from 'react-router-dom'

type BrowserTableProps = {
    data: Project[] | System[]
    title: string
    description: string
}

const headers: DataTableHeader<string>[] = [
    {
        key: 'name',
        header: 'Naam',
    },
    {
        key: 'description',
        header: 'Beschrijving',
    },
]

export const BrowserTable: React.FC<BrowserTableProps> = ({
    title,
    description,
    data,
}) => {
    const navigate = useNavigate()
    const location = useLocation()
    const prepend = location.pathname == '/models' ? '' : location.pathname

    return (
        <DataTable
            rows={data as any}
            headers={headers}
            render={({ rows, headers, getHeaderProps }) => (
                <TableContainer title={title} description={description}>
                    <Table>
                        <TableHead>
                            <TableRow>
                                {headers.map((header) => (
                                    <TableHeader
                                        {...getHeaderProps({ header })}
                                    >
                                        {header.header}
                                    </TableHeader>
                                ))}
                            </TableRow>
                        </TableHead>
                        <TableBody>
                            {rows.map((row) => (
                                <TableRow
                                    key={row.id}
                                    style={{ cursor: 'pointer' }}
                                    onClick={() =>
                                        navigate(`${prepend}/${row.id}`)
                                    }
                                >
                                    {row.cells.map((cell) =>
                                        cell.id.split(':')[1] ===
                                        'id' ? null : (
                                            <TableCell key={cell.id}>
                                                {cell.value}
                                            </TableCell>
                                        )
                                    )}
                                </TableRow>
                            ))}
                        </TableBody>
                    </Table>
                </TableContainer>
            )}
        />
    )
}

export default BrowserTable
