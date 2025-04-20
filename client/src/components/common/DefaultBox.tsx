import { Box, BoxProps } from '@mui/material';

function DefaultBox(props: BoxProps) {
  return (
    <Box
      {...props}
      sx={{
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center', // Horizontal centering
        justifyContent: 'center', // Vertical centering
        minHeight: '100vh', // Ensure it takes full height
        width: '100%', // Full width
        bgcolor: 'background.default',
        ...props.sx,
      }}
    />
  );
}

export default DefaultBox;
