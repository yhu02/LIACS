import styled from "styled-components";

export const Button = styled.button`
   background: #bbaae5;
   border-radius: ${({ theme }) => theme.borderRadius};
   box-shadow: ${({ theme }) => theme.boxShadow};
   transition: ${({ theme }) => theme.transition};
   color: ${({ theme }) => theme.text1};
   border: 0px solid ${({ theme }) => theme.primary};
   padding: 1rem 2.2rem;
   font-size: clamp(1rem, 2vw, 1.3rem);
   text-transform: capitalize;
   font-weight: 500;
   width: 100%;

   /* disabled styles */
   cursor: ${({ disabled }) => disabled ? 'not-allowed' : 'pointer'};
   background: ${({ disabled }) => disabled ? '#ffffff26' : ''};

   &.ans{
      background:  #00d2d3;
      color: ${({ theme }) => theme.textDark1};
   }
   &.secondary{
      background: #00d2d3;
      color: ${({ theme }) => theme.textDark1};
      background: ${({ disabled }) => disabled ? '#ffffff26' : ''};
   }

   @media screen{

      :hover{
         filter: brightness(0.8);
      }
   }
   
   :active{
      transform: scaleX(.9);
   }

   &.selected{
      background: orangered;
   }
`

export const Button2 = styled.button`
   background-color: '#5f27cd00',
   border-radius: '20px',
   box-shadow: ${({ theme }) => theme.boxShadow};
   transition: ${({ theme }) => theme.transition};
   color: ${({ theme }) => theme.text1};
   border: 'solid 7px #5f27cd',
   padding: 1rem 2.2rem;
   font-size: clamp(1rem, 2vw, 1.3rem);
   text-transform: capitalize;
   font-weight: 500;
   width: 100%;
   height: '100%',

   /* disabled styles */
   cursor: ${({ disabled }) => disabled ? 'not-allowed' : 'pointer'};
   background: ${({ disabled }) => disabled ? '#ffffff26' : ''};

   &.ans{
      background:  #00d2d3;
      color: ${({ theme }) => theme.textDark1};
   }
   &.secondary{
      background: #00d2d3;
      color: ${({ theme }) => theme.textDark1};
      background: ${({ disabled }) => disabled ? '#ffffff26' : ''};
   }

   @media screen{

      :hover{
         filter: brightness(0.8);
      }
   }
   &:hover: { opacity: 0.72 },
   :active{
      transform: scaleX(.9);
   }

   &.selected{
      background: orangered;
   }
`

export const Box = styled.div`

   display: flex;
   flex-direction: column;
   gap: 1rem;
   justify-content: space-between;
   /* padding: ${({ theme }) => theme.padding}; */

   :last-child{
      margin-bottom: 4rem;
   }

   @media screen and (min-width: 980px){
      min-block-size: 50rem;
      inline-size: 50rem;
   }

`